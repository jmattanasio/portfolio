#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <pthread.h>
#include "bonding.h"

// Struct representing a node in the list (either hydrogen or oxygen atom waiting to bond)
typedef struct list_node {
    int id;                         // ID of the atom
    int h1, h2, o;                  // IDs involved in the bond (two hydrogens, one oxygen)
    pthread_cond_t *cond;            // Condition variable for this atom to wait on
    struct list_node *next;          // Next node in the list
    struct list_node *prev;          // Previous node in the list
} list_node_t;

// Struct to hold global/shared resources
typedef struct globals {
    list_node_t *h_list;             // Linked list of waiting hydrogen atoms
    list_node_t *o_list;             // Linked list of waiting oxygen atoms
    pthread_mutex_t *lock;           // Mutex to protect shared resources
    char *verbosity;                 // Verbosity flags for logging
} globals_t;

// --- Helper Functions ---

// Bonds two hydrogens with an oxygen, setting their bond IDs
static void bond_hydrogen_with(globals_t *g, list_node_t *self, list_node_t *partner, list_node_t *oxygen) {
    self->h1 = partner->id;
    self->h2 = self->id;
    self->o = oxygen->id;

    partner->h1 = partner->id;
    partner->h2 = self->id;
    partner->o = oxygen->id;

    oxygen->h1 = partner->id;
    oxygen->h2 = self->id;
    oxygen->o = oxygen->id;
}

// Bonds an oxygen atom with two hydrogens
static void bond_oxygen_with(globals_t *g, list_node_t *self, list_node_t *h1, list_node_t *h2) {
    self->h1 = h1->id;
    self->h2 = h2->id;
    self->o = self->id;

    h1->h1 = h1->id;
    h1->h2 = h2->id;
    h1->o = self->id;

    h2->h1 = h1->id;
    h2->h2 = h2->id;
    h2->o = self->id;
}

// Removes the head node from a list
static void remove_node(list_node_t **head) {
    if (*head) {
        *head = (*head)->next;
        if (*head) (*head)->prev = NULL;
    }
}

// Adds a node to the end of a list
static void add_node_to_list(list_node_t **head, list_node_t *node) {
    if (!*head) {
        *head = node;
    } else {
        list_node_t *curr = *head;
        while (curr->next) curr = curr->next;
        curr->next = node;
        node->prev = curr;
    }
}

// Logs messages if verbosity flag 'A' is set
static void verbose_log(char *verbosity, const char *format, int id) {
    if (strchr(verbosity, 'A')) {
        printf(format, id);
    }
}

// --- Main Functions ---

// Initializes global resources (mutex, lists, verbosity settings)
void *initialize_v(char *verbosity) {
    globals_t *g = malloc(sizeof(globals_t));
    if (!g) {
        perror("Failed to allocate globals");
        exit(1);
    }
    g->h_list = NULL;
    g->o_list = NULL;
    g->lock = new_mutex();
    g->verbosity = verbosity;
    return g;
}

// Behavior for a hydrogen thread
void *hydrogen(void *arg) {
    struct bonding_arg *a = (struct bonding_arg *) arg;
    globals_t *g = (globals_t *) a->v;

    // Create a list_node representing this hydrogen atom
    list_node_t self = {
        .id = a->id,
        .h1 = -1,
        .h2 = -1,
        .o = -1,
        .cond = new_cond(),
        .next = NULL,
        .prev = NULL
    };

    pthread_mutex_lock(g->lock);

    // Check if there is an oxygen and another hydrogen ready to bond
    if (g->o_list && g->h_list) {
        verbose_log(g->verbosity, "Hydrogen Thread %d found bond\n", self.id);
        bond_hydrogen_with(g, &self, g->h_list, g->o_list);

        // Wake up partner hydrogen and oxygen
        pthread_cond_signal(g->h_list->cond);
        pthread_cond_signal(g->o_list->cond);

        // Remove them from their respective lists
        remove_node(&g->h_list);
        remove_node(&g->o_list);

        pthread_mutex_unlock(g->lock);
        free(self.cond);
        return (void *) Bond(self.h1, self.h2, self.o);
    }

    // Otherwise, wait by adding self to hydrogen list
    add_node_to_list(&g->h_list, &self);

    verbose_log(g->verbosity, "Hydrogen Thread %d waiting\n", self.id);
    pthread_cond_wait(self.cond, g->lock);   // Wait until another thread signals

    verbose_log(g->verbosity, "Hydrogen Thread %d being bonded\n", self.id);

    pthread_mutex_unlock(g->lock);
    free(self.cond);
    return (void *) Bond(self.h1, self.h2, self.o);
}

// Behavior for an oxygen thread
void *oxygen(void *arg) {
    struct bonding_arg *a = (struct bonding_arg *) arg;
    globals_t *g = (globals_t *) a->v;

    // Create a list_node representing this oxygen atom
    list_node_t self = {
        .id = a->id,
        .h1 = -1,
        .h2 = -1,
        .o = -1,
        .cond = new_cond(),
        .next = NULL,
        .prev = NULL
    };

    pthread_mutex_lock(g->lock);

    // Check if there are at least two hydrogens ready
    if (g->h_list && g->h_list->next) {
        verbose_log(g->verbosity, "Oxygen Thread %d found bond\n", self.id);
        bond_oxygen_with(g, &self, g->h_list, g->h_list->next);

        // Wake up the two hydrogen threads
        pthread_cond_signal(g->h_list->cond);
        pthread_cond_signal(g->h_list->next->cond);

        // Remove both hydrogens from the list
        remove_node(&g->h_list);
        remove_node(&g->h_list);

        pthread_mutex_unlock(g->lock);
        free(self.cond);
        return (void *) Bond(self.h1, self.h2, self.o);
    }

    // Otherwise, wait by adding self to oxygen list
    add_node_to_list(&g->o_list, &self);

    verbose_log(g->verbosity, "Oxygen Thread %d waiting\n", self.id);
    pthread_cond_wait(self.cond, g->lock);   // Wait until another thread signals

    verbose_log(g->verbosity, "Oxygen Thread %d being bonded\n", self.id);

    pthread_mutex_unlock(g->lock);
    free(self.cond);
    return (void *) Bond(self.h1, self.h2, self.o);
}