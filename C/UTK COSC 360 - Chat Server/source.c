#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <pthread.h>
#include <signal.h>
#include "dllist.h"
#include "jrb.h"
#include "sockettome.h"

// ----------- STRUCT DEFINITIONS -----------

// ChatServer holds a map of chat rooms by name
typedef struct ChatServer {
    JRB room_map;
} ChatServer;

// ChatRoom contains its name, list of connected clients, and a message queue.
// It also includes thread-related structures for concurrent access and signaling.
typedef struct ChatRoom {
    char* room_name;
    Dllist clients;
    Dllist message_queue;

    pthread_t thread;
    pthread_mutex_t *mutex;
    pthread_cond_t *message_ready;
} ChatRoom;

// ChatClient holds information for each connected user including their room,
// thread, socket, input/output file streams, and username.
typedef struct ChatClient {
    ChatRoom* room;
    pthread_t thread;

    int socket_fd;
    FILE* in_stream;
    FILE* out_stream;
    char username[100];
} ChatClient;

// Global pointer to server
ChatServer* server = NULL;

// Thread function declarations
void* handle_room(void* arg);
void* handle_client(void* arg);

// Core chat functionality
ChatRoom* join_room(ChatClient* client, const char* name);
void broadcast_message(ChatClient* sender, const char* message);

// Cleanup and shutdown functions
void cleanup_client(ChatClient* client);
void cleanup_room(ChatRoom* room);
void shutdown_server(int signal);

// ----------- MAIN FUNCTION -----------

int main(int argc, char **argv) {
    if (argc < 3) {
        fprintf(stderr, "usage: %s port Room1 Room2 ...\n", argv[0]);
        exit(1);
    }

    signal(SIGINT, shutdown_server); // Handle Ctrl+C to clean up

    server = malloc(sizeof(ChatServer));
    server->room_map = make_jrb(); // Initialize room map

    // Create threads for each chat room
    for (int i = 2; i < argc; ++i) {
        ChatRoom* room = malloc(sizeof(ChatRoom));
        room->room_name = strdup(argv[i]);
        room->clients = new_dllist();
        room->message_queue = new_dllist();
        room->mutex = malloc(sizeof(pthread_mutex_t));
        room->message_ready = malloc(sizeof(pthread_cond_t));
        pthread_mutex_init(room->mutex, NULL);
        pthread_cond_init(room->message_ready, NULL);

        jrb_insert_str(server->room_map, room->room_name, new_jval_v(room));

        // Start thread for handling room messages
        if (pthread_create(&room->thread, NULL, handle_room, room) != 0) {
            perror("pthread_create");
            exit(1);
        }
    }

    // Open listener socket
    int port = atoi(argv[1]);
    printf("Server listening on port: %d\n", port);

    int listener = serve_socket(port);
    while (1) {
        int client_fd = accept_connection(listener);
        ChatClient* client = malloc(sizeof(ChatClient));
        client->socket_fd = client_fd;

        // Start thread to handle client communication
        if (pthread_create(&client->thread, NULL, handle_client, client) != 0) {
            perror("pthread_create");
            exit(1);
        }
    }

    // Should never be reached
    shutdown_server(0);
    return 0;
}

// ----------- SIGNAL HANDLER -----------

// Gracefully shuts down the server
void shutdown_server(int signal) {
    JRB node;
    jrb_traverse(node, server->room_map) {
        ChatRoom* room = node->val.v;
        pthread_detach(room->thread);  // Detach thread to avoid resource leaks
        cleanup_room(room);            // Free memory/resources
    }

    jrb_free_tree(server->room_map);
    free(server);
    exit(0);
}

// ----------- ROOM THREAD -----------

// Continuously listens for messages in the queue and broadcasts them to clients
void* handle_room(void* arg) {
    ChatRoom* room = arg;

    while (1) {
        pthread_mutex_lock(room->mutex);
        while (dll_empty(room->message_queue)) {
            pthread_cond_wait(room->message_ready, room->mutex); // Wait for new messages
        }

        // Send all messages to each client
        Dllist msg_node;
        dll_traverse(msg_node, room->message_queue) {
            char* message = msg_node->val.s;
            Dllist client_node;
            dll_traverse(client_node, room->clients) {
                ChatClient* client = client_node->val.v;
                fprintf(client->out_stream, "%s", message);
                fflush(client->out_stream);
            }
        }

        // Free message strings after sending
        dll_traverse(msg_node, room->message_queue) {
            free(msg_node->val.s);
        }

        // Reset message queue
        free_dllist(room->message_queue);
        room->message_queue = new_dllist();

        pthread_mutex_unlock(room->mutex);
    }

    return NULL;
}

// ----------- CLIENT THREAD -----------

// Handles user interaction: room selection, message sending, disconnection
void* handle_client(void* arg) {
    ChatClient* client = arg;
    char buffer[350];
    Dllist node;
    JRB room_node;

    // Open read/write streams from client socket
    client->in_stream = fdopen(client->socket_fd, "r");
    client->out_stream = fdopen(client->socket_fd, "w");
    if (!client->in_stream || !client->out_stream) {
        perror("fdopen");
        cleanup_client(client);
        return NULL;
    }

    // Show available rooms and current users
    fprintf(client->out_stream, "Chat Rooms:\n\n");
    jrb_traverse(room_node, server->room_map) {
        ChatRoom* room = room_node->val.v;
        sprintf(buffer, "%s:", room->room_name);

        pthread_mutex_lock(room->mutex);
        dll_traverse(node, room->clients) {
            ChatClient* c = node->val.v;
            strcat(buffer, " ");
            strcat(buffer, c->username);
        }
        pthread_mutex_unlock(room->mutex);

        strcat(buffer, "\n");
        fprintf(client->out_stream, "%s", buffer);
    }

    // Get user's name
    fprintf(client->out_stream, "\nEnter your chat name (no spaces):\n");
    fflush(client->out_stream);
    if (fscanf(client->in_stream, "%s", client->username) != 1) {
        cleanup_client(client);
        return NULL;
    }

    // Prompt for chat room selection
    while (1) {
        fprintf(client->out_stream, "Enter chat room:\n");
        fflush(client->out_stream);
        if (fscanf(client->in_stream, "%s", buffer) != 1) {
            cleanup_client(client);
            return NULL;
        }

        client->room = join_room(client, buffer);
        if (client->room != NULL) break;

        fprintf(client->out_stream, "No chat room %s.\n", buffer);
    }

    // Notify other users
    sprintf(buffer, "%s has joined\n", client->username);
    broadcast_message(client, buffer);

    // Main loop: read messages from user and broadcast
    char line[300];
    while (fgets(line, sizeof(line), client->in_stream)) {
        if (strcmp(line, "\n") == 0) continue; // Skip empty messages
        sprintf(buffer, "%s: %s", client->username, line);
        broadcast_message(client, buffer);
    }

    // Client disconnected: remove from client list
    pthread_mutex_lock(client->room->mutex);
    dll_traverse(node, client->room->clients) {
        ChatClient* c = node->val.v;
        if (strcmp(client->username, c->username) == 0) {
            dll_delete_node(node);
            break;
        }
    }

    // Broadcast leave message
    sprintf(buffer, "%s has left\n", client->username);
    dll_append(client->room->message_queue, new_jval_s(strdup(buffer)));
    pthread_cond_signal(client->room->message_ready);
    pthread_mutex_unlock(client->room->mutex);

    cleanup_client(client);
    return NULL;
}

// ----------- JOIN ROOM -----------

// Adds client to room if it exists
ChatRoom* join_room(ChatClient* client, const char* name) {
    JRB node;
    jrb_traverse(node, server->room_map) {
        ChatRoom* room = node->val.v;
        if (strcmp(room->room_name, name) == 0) {
            pthread_mutex_lock(room->mutex);
            dll_append(room->clients, new_jval_v(client));
            pthread_mutex_unlock(room->mutex);
            return room;
        }
    }
    return NULL; // Room not found
}

// ----------- BROADCAST MESSAGE -----------

// Adds a message to the room's queue and signals the room thread
void broadcast_message(ChatClient* sender, const char* msg) {
    pthread_mutex_lock(sender->room->mutex);
    dll_append(sender->room->message_queue, new_jval_s(strdup(msg)));
    pthread_cond_signal(sender->room->message_ready);
    pthread_mutex_unlock(sender->room->mutex);
}

// ----------- CLEANUP -----------

// Frees client resources
void cleanup_client(ChatClient* client) {
    if (client->socket_fd >= 0) close(client->socket_fd);
    if (client->in_stream) fclose(client->in_stream);
    if (client->out_stream) fclose(client->out_stream);
    free(client);
}

// Frees room resources and destroys synchronization primitives
void cleanup_room(ChatRoom* room) {
    Dllist tmp;
    if (room->message_queue) {
        dll_traverse(tmp, room->message_queue)
            free(tmp->val.s);
        free_dllist(room->message_queue);
    }
    if (room->clients)
        free_dllist(room->clients);

    pthread_mutex_destroy(room->mutex);
    pthread_cond_destroy(room->message_ready);
    free(room->mutex);
    free(room->message_ready);
    free(room->room_name);
    free(room);
}