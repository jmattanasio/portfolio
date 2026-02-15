#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "fields.h"
#include "jrb.h"
#include "jval.h"

typedef struct person {
    char *name;
    int sex; // 0 for men, 1 for women
    char *father;
    char *mother;
    char **children;
    int visited; // dfs
    int num_parents;
    int num_parents_printed; // compare this with num_parents to 
} Person;

Jval jv;
JRB people;
int line_number_tracking = 0;


Person* create_person(const char *name) {
    Person *new_person = (Person *)malloc(sizeof(Person));
    if (!new_person) {
        perror("Memory allocation failed");
        exit(EXIT_FAILURE);
    }
    new_person->name = strdup(name); // Dynamically allocate memory for name
    if (!new_person->name) {
        perror("Memory allocation failed");
        exit(EXIT_FAILURE);
    }
    new_person->sex = -1;
    new_person->father = NULL;
    new_person->mother = NULL;
    new_person->children = NULL;
    new_person->visited = 0;
    new_person->num_parents = 0;
    new_person->num_parents_printed = 0;

    jv.v = new_person;
    jrb_insert_str(people, new_person->name, new_jval_v(new_person));


    return new_person;
}

void cleanup() {
    JRB node;
    jrb_traverse(node, people) {
        Person *person = (Person *)jval_v(node->val);
        free(person->name);
        if (person->father) free(person->father);
        if (person->mother) free(person->mother);
        if (person->children) {
            for (int i = 0; person->children[i]; i++) {
                free(person->children[i]);
            }
            free(person->children);
        }
        free(person);
    }
    jrb_free_tree(people);
}


Person* check_for_person(const char *name) {
    JRB node = jrb_find_str(people, (char *)name);
    if (node) {
        return (Person *)jval_v(node->val);
    } else {
        return create_person(name);
    }
}

void add_sex(Person *person, int sex_given, int line_number){
    if(person->sex == 0 && sex_given == 1){
        fprintf(stderr, "Bad input - sex mismatch on line %d\n", line_number);
        cleanup();
        exit(EXIT_FAILURE);}
    else if(person->sex == 1 && sex_given == 0){
        fprintf(stderr, "Bad input - sex mismatch on line %d\n", line_number);
        cleanup();
        exit(EXIT_FAILURE);
    }
    person->sex = sex_given;
}

void add_child(Person *parent, const char *child_name) {
    Person *child = check_for_person(child_name);
    // Add child to parent's children list (not implemented in the original code)
    if (parent->children == NULL) {
        parent->children = (char **)malloc(sizeof(char *));
        if (!parent->children) {
            perror("Memory allocation failed");
            exit(EXIT_FAILURE);
        }
        parent->children[0] = strdup(child_name);
    } else {
        int i;
        for (i = 0; parent->children[i]; i++);
        parent->children = (char **)realloc(parent->children, (i + 2) * sizeof(char *));
        if (!parent->children) {
            perror("Memory allocation failed");
            exit(EXIT_FAILURE);
        }
        parent->children[i] = strdup(child_name);
        parent->children[i + 1] = NULL;
    }
}


void add_bidirectional_father_of(Person *person, Person *child) {
    add_child(person, child->name);
    child->father = strdup(person->name);
    person->sex = 0;
}

void add_bidirectional_mother_of(Person *person, Person *child) {
    add_child(person, child->name);
    child->mother = strdup(person->name);
    person->sex = 1;
}
void add_father(Person *person, const char *father_name, int line_number) {
    if(person->father != NULL){
        fprintf(stderr, "Bad input -- child with two fathers on line %d\n", line_number);
        cleanup();
        exit(EXIT_FAILURE);
    }
    else if (person->father == NULL){
        person->father = strdup(father_name);
        Person *father = check_for_person(father_name);
        add_child(check_for_person(father_name), person->name); //add a child to the father, bidirectional link
        father->sex = 0;
        add_sex(father, 0, line_number_tracking);
    }
}

void add_mother(Person *person, const char *mother_name, int line_number) {
    if (person->mother != NULL){
        fprintf(stderr, "Bad input -- child with two mothers on line %d\n", line_number);
        cleanup();
        exit(EXIT_FAILURE);
    }
    else if(person->mother == NULL){
    person->mother = strdup(mother_name);
    Person *mother = check_for_person(mother_name);
    add_child(check_for_person(mother_name), person->name); //same here
    mother->sex = 1;
    add_sex(mother, 1, line_number_tracking);
}
}




void dfs_check_cycle(Person *person, JRB visited) {
    if (jrb_find_str(visited, person->name)) {
        fprintf(stderr, "Bad input -- cycle in specification\n");
        cleanup();
        exit(EXIT_FAILURE);
    }

    jrb_insert_str(visited, person->name, new_jval_v(NULL));

    if (person->father) {
        Person *father = check_for_person(person->father);
        dfs_check_cycle(father, visited);
    }

    if (person->mother) {
        Person *mother = check_for_person(person->mother);
        dfs_check_cycle(mother, visited);
    }

    jrb_delete_node(jrb_find_str(visited, person->name));
}

void check_for_cycles() {
    JRB node;
    jrb_traverse(node, people) {
        Person *person = (Person *)jval_v(node->val);
        JRB visited = make_jrb();
        dfs_check_cycle(person, visited);
        jrb_free_tree(visited);
    }
}

void print_people() {
    JRB node;
    int printed_someone;

    jrb_traverse(node, people) {
    Person *person = (Person *)jval_v(node->val);
    if (person->father){
        person->num_parents++;
    }
    if (person->mother){
        person->num_parents++;
    }
    }
    do {
        printed_someone = 0;
        jrb_traverse(node, people) {
            Person *person = (Person *)jval_v(node->val);
            if (person->visited == 0 && person->num_parents == person->num_parents_printed) {
                //print the person
                printf("%s\n", person->name);
                if (person->sex == 0) {//--------------------------sex
                    printf("  Sex: Male\n");
             }else if (person->sex == 1) {
                 printf("  Sex: Female\n");
             }else{
                 printf("  Sex: Unknown\n");
             }
             if (person->father) {//--------------------------father
                 printf("  Father: %s\n", person->father);
             } else{
                 printf("  Father: Unknown\n");
             }
             if (person->mother) {//--------------------------mother
                 printf("  Mother: %s\n", person->mother);
             } else{
                 printf("  Mother: Unknown\n");
             }
             if (person->children) {//--------------------------children
                 printf("  Children:\n");
                 for (int i = 0; person->children[i]; i++) {
                     printf("    %s\n", person->children[i]);
                 }


             } else {
                 printf("  Children: None\n");
             }
             printf("\n");

             // Print children (not implemented in the original code)
             person->visited = 1;
             printed_someone = 1;

             if (person->children){
                 for (int i = 0; person->children[i]; i++) {
                     Person *child = check_for_person(person->children[i]);
                     child->num_parents_printed++;
                     }
                }
         }
        }
        
    } while (printed_someone != 0);
    printf("\n");
}



int main(int argc, char **argv) {
    IS is;
    int i; //for the "for loop" below

    if (argc != 1) {
        fprintf(stderr, "Usage: %s\n", argv[0]);
        exit(1);
    }

    people = make_jrb();

    is = new_inputstruct(NULL);
    if (is == NULL) {
        perror("stdin");
        exit(1);
    }

    Person *head = NULL;

    while (get_line(is) >= 0) { // Read each line from input
        for (i = 0; i < is->NF; i++) { // Iterate through each field in the line
            line_number_tracking++;
            if (strcmp(is->fields[i], "PERSON") == 0) { // Check if the field is "PERSON"
                char full_name[256] = ""; // Initialize full_name buffer
                for (int j = i + 1; j < is->NF; j++) { // Concatenate the rest of the fields to form the full name
                    if (j > i + 1) strcat(full_name, " "); // Add space between names
                    strcat(full_name, is->fields[j]); // Append the field to full_name
                }

                head = check_for_person(full_name); // Check if the person exists and set head to that person
                i = is->NF - 1; // Skip the rest of the fields in this line 

            } else if (strcmp(is->fields[i], "FATHER") == 0) { // Check if the field is "FATHER"
                if (head == NULL) continue; // If no person is set, continue to the next field
                char full_name[256] = ""; // Initialize full_name buffer
                for (int j = i + 1; j < is->NF; j++) { // Concatenate the rest of the fields to form the full name
                    if (j > i + 1) strcat(full_name, " "); // Add space between names
                    strcat(full_name, is->fields[j]); // Append the field to full_name
                }

                add_father(head, full_name, is->line); // Add the father to the current person
                i = is->NF - 1; // Skip the rest of the fields in this line

            } else if (strcmp(is->fields[i], "MOTHER") == 0) { // Check if the field is "MOTHER"
                if (head == NULL) continue; // If no person is set, continue to the next field
                char full_name[256] = ""; // Initialize full_name buffer
                for (int j = i + 1; j < is->NF; j++) { // Concatenate the rest of the fields to form the full name
                    if (j > i + 1) strcat(full_name, " "); // Add space between names
                    strcat(full_name, is->fields[j]); // Append the field to full_name
                }

                add_mother(head, full_name, is->line); // Add the mother to the current person
                i = is->NF - 1; // Skip the rest of the fields in this line

            } else if (strcmp(is->fields[i], "FATHER_OF") == 0) { // Check if the field is "FATHER_OF" or "MOTHER_OF"
                if (head == NULL) continue; // If no person is set, continue to the next field
                char full_name[256] = ""; // Initialize full_name buffer
                for (int j = i + 1; j < is->NF; j++) { // Concatenate the rest of the fields to form the full name
                    if (j > i + 1) strcat(full_name, " "); // Add space between names
                    strcat(full_name, is->fields[j]); // Append the field to full_name
                }

                add_bidirectional_father_of(head, check_for_person(full_name)); // Add the father of the current person
                i = is->NF - 1; // Skip the rest of the fields in this line
            } else if (strcmp(is->fields[i], "MOTHER_OF") == 0) { // Check if the field is "FATHER_OF" or "MOTHER_OF"
                if (head == NULL) continue; // If no person is set, continue to the next field
                char full_name[256] = ""; // Initialize full_name buffer
                for (int j = i + 1; j < is->NF; j++) { // Concatenate the rest of the fields to form the full name
                    if (j > i + 1) strcat(full_name, " "); // Add space between names
                    strcat(full_name, is->fields[j]); // Append the field to full_name
                }

                add_bidirectional_mother_of(head, check_for_person(full_name)); // Add the father of the current person
                i = is->NF - 1; // Skip the rest of the fields in this line
            } else if (strcmp(is->fields[i], "SEX") == 0) { // Check if the field is
                if (head == NULL) continue; // If no person is set, continue to the next field
                if (strcmp(is->fields[i + 1], "M") == 0) {
                    add_sex(head, 0, is->line);
                } else if (strcmp(is->fields[i + 1], "F") == 0) {
                    add_sex(head, 1, is->line);
                }
            }
        }
    }

    check_for_cycles(); // Check for cycles in the family tree
    print_people(); // Print the family tree
    cleanup(); // Free memory
    return 0;
}