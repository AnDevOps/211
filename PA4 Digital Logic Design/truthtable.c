/* CS 211 PA4
 * Created for: abl101
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

// struct definitions
typedef enum { AND, OR, NAND, NOR, XOR, NOT, PASS, DECODER, MULTIPLEXER } kind_t;

struct LinkedList { 
    char string[17];
    int value;
    struct LinkedList* next;
};
typedef struct LinkedList Node;

struct gate {
    kind_t kind;
    int size;
    Node *params;
};
typedef struct gate Gate;

struct GateLinkedList {
    Gate *node;
    struct GateLinkedList *next;
};
typedef struct GateLinkedList GateLL;

Node* insert(char str[17], Node *root, int val) { 
    Node *temp = malloc(sizeof(Node));
    strcpy(temp->string, str);
    temp->next = NULL;
    temp->value = -1;
    if(val != -1) temp->value = val;

    if(root == NULL) return temp;
    
    Node *ptr = root;

    while(ptr->next != NULL) {
        ptr = ptr->next;
    }

    ptr->next = temp;
    return root;
}

Node* insertInteger(int dec, Node *root) {
    Node *temp = malloc(sizeof(Node));
    strcpy(temp->string, "abl101");
    temp->next = NULL;
    temp->value = dec;

    if(root == NULL) return temp;
    
    Node *ptr = root;

    while(ptr->next != NULL) {
        ptr = ptr->next;
    }

    ptr->next = temp;
    return root;
}

GateLL* insertGate(GateLL *root, kind_t en, int val, FILE *parse_file) {
    // create actual Gate
    Gate *tempGate = malloc(sizeof(Gate));
    tempGate->kind = en;
    tempGate->size = val;
    tempGate->params = NULL;

    int i;
    char temp_name[17];

    for(i = 0; i < val; i++) {
        fscanf(parse_file, " %16s", temp_name);

        if(strcmp(temp_name, "1") == 0) {
            tempGate->params = insertInteger(1, tempGate->params);
        } else if(strcmp(temp_name, "0") == 0) {
            tempGate->params = insertInteger(0, tempGate->params);
        } else {
            tempGate->params = insert(temp_name, tempGate->params, -1);
        }
    }


    // make temp GateLL
    GateLL *temp = malloc(sizeof(GateLL));
    temp->node = tempGate;
    temp->next = NULL;

    if(root == NULL) return temp;

    GateLL *ptr = root;

    while(ptr->next != NULL) {
        ptr = ptr->next;
    }

    ptr->next = temp;
    return root;
}

GateLL* insertDecoderGate(GateLL *root, kind_t en, FILE *parse_file) {
    // create decoder gate
    Gate *tempGate = malloc(sizeof(Gate));
    tempGate->kind = en;

    int inputSize;
    fscanf(parse_file, "%d", &inputSize);
    tempGate->size = inputSize;
    int totalSize = inputSize + pow(2, inputSize);
    tempGate->params = NULL;

    int i;
    char temp_name[17];

    for(i = 0; i < totalSize; i++) {
        fscanf(parse_file, " %16s", temp_name);
        
        if(strcmp(temp_name, "1") == 0) {
            tempGate->params = insertInteger(1, tempGate->params);
        } else if(strcmp(temp_name, "0") == 0) {
            tempGate->params = insertInteger(0, tempGate->params);
        } else {
            tempGate->params = insert(temp_name, tempGate->params, -1);
        }
    }

    // make temp GateLL
    GateLL *temp = malloc(sizeof(GateLL));
    temp->node = tempGate;
    temp->next = NULL;

    if(root == NULL) return temp;

    GateLL *ptr = root;

    while(ptr->next != NULL) {
        ptr = ptr->next;
    }

    ptr->next = temp;
    return root;
}

GateLL *insertMultiplexerGate(GateLL *root, kind_t en, FILE *parse_file) {
    // create multiplexer gate
    Gate *temp_gate = malloc(sizeof(Gate));
    temp_gate->kind = en;

    
    // reading the input size   
    int input_size;
    fscanf(parse_file, "%d", &input_size);
    
    temp_gate->size = input_size;
    temp_gate->params = NULL;

    int i;
    char temp_name[17];
    
    for(i = 0; i < pow(2, input_size) + input_size + 1; i++) {
        fscanf(parse_file, " %16s", temp_name);

        if(strcmp(temp_name, "1") == 0) {
            temp_gate->params = insertInteger(1, temp_gate->params);
        } else if(strcmp(temp_name, "0") == 0) {
            temp_gate->params = insertInteger(0, temp_gate->params);
        } else {
            temp_gate->params = insert(temp_name, temp_gate->params, -1);
        }
    }

    // make temp GateLL
    GateLL *temp = malloc(sizeof(GateLL));
    temp->node = temp_gate;
    temp->next = NULL;

    if(root == NULL) return temp;

    
    GateLL *ptr = root;

    while(ptr->next != NULL) {
        ptr = ptr->next;
    }

    ptr->next = temp;
    return root;

}

int verifyVal1(GateLL *root, Node *temp_values, Node *input_values) {
    if(strcmp(root->node->params->string, "abl101") == 0) {
        return root->node->params->value;
    }
    
    Node *ptr;
    for(ptr = input_values; ptr != NULL; ptr = ptr->next) {
        if(strcmp(root->node->params->string, ptr->string) == 0) return ptr->value;
    }

    for(ptr = temp_values; ptr != NULL; ptr = ptr->next) {
        if(strcmp(root->node->params->string, ptr->string) == 0) return ptr->value;
    }


    return -100;
}

int verifyVal2(GateLL *root, Node *temp_values, Node *input_values) {
    if(strcmp(root->node->params->next->string, "abl101") == 0) {
        return root->node->params->next->value;
    }
    Node *ptr;
    for(ptr = input_values; ptr != NULL; ptr = ptr->next) {
        if(strcmp(root->node->params->next->string, ptr->string) == 0) return ptr->value;
        
    }

    for(ptr = temp_values; ptr != NULL; ptr = ptr->next) {
        if(strcmp(root->node->params->next->string, ptr->string) == 0) return ptr->value;
    }

    return -100;
}

int verifyMUXValue(Node *temp_values, Node *input_values, Node *current_list) {
    if(strcmp(current_list->string, "abl101") == 0) {
        return current_list->value;
    }

    Node *ptr;
    for(ptr = input_values; ptr != NULL; ptr = ptr->next) {
        if(strcmp(current_list->string, ptr->string) == 0) return ptr->value;
    }

    for(ptr = temp_values; ptr != NULL; ptr = ptr->next) {
        if(strcmp(current_list->string, ptr->string) == 0) return ptr->value;
    }

    return -100;
}

int IntegerConversionInput(int value) {
    int temp, base_value = 0, base = 1;
    
    while(value > 0) {
        temp = value % 10;
        base_value += temp * base;
        value /= 10;
        base *= 2;
    }

    return base_value;
}

Node *binaryConversionInput(int value, Node *root, int currentLoop) {
    int j;
    Node *ptr = root;
    for(j = value - 1; j >= 0; j--) {
        int binary = currentLoop >> j;

        if(binary & 1) {
            ptr->value = 1;
            ptr = ptr->next;
        } else {
            ptr->value = 0;
            ptr = ptr->next;
        }
    }

    return root;
}

Node *updateOutput(Node *calculated, Node *output) {
    Node *ptr, *output_ptr;

    for(ptr = calculated; ptr != NULL; ptr= ptr->next) {
        for(output_ptr = output; output_ptr != NULL; output_ptr = output_ptr->next) {
            if(strcmp(ptr->string, output_ptr->string) == 0) {
                output_ptr->value = ptr->value;
            }
        }
    }

    return output;

}

GateLL *updateValue(Node *input_stream, GateLL *root) {
    Node *ptr, *value_ptr;
    GateLL *gate_ptr;

    for(ptr = input_stream; ptr != NULL; ptr = ptr->next) {
        for(gate_ptr = root; gate_ptr != NULL; gate_ptr = gate_ptr->next) {
            for(value_ptr = gate_ptr->node->params; value_ptr != NULL; value_ptr = value_ptr->next) {
                if(strcmp(value_ptr->string, "abl101") == 0) continue;
                if(strcmp(value_ptr->string, ptr->string) == 0) value_ptr->value = ptr->value;  
            }
        }
    }

    return root;
}

int LLToInteger(Node *input_values, GateLL *root, Node*temp_values) {
    Node *ptr = root->node->params;

    if(root->node->kind == MULTIPLEXER) {
        int k;
        for(k = 0; k < pow(2, root->node->size); k++) {
            ptr = ptr->next;
        }
    }

    Node *pointer;
    Node *new_list = NULL;

    int i, key = 0;
    for(i = 0; i < root->node->size; i++) { // search inputs 
        for(pointer = input_values; pointer != NULL; pointer = pointer->next) {
            if(strcmp(ptr->string, pointer->string) == 0) {
                new_list = insert(ptr->string, new_list, pointer->value);
                key = 1;
                break;
            }
        }

        if(key == 0) {
            for(pointer = temp_values; pointer != NULL; pointer = pointer->next) {
                if(strcmp(ptr->string, pointer->string) == 0) {
                    new_list = insert(ptr->string, new_list, pointer->value);
                    break;
                }
            }
        }

        key = 0;
        ptr = ptr->next;
    }

    
    Node *new_ptr = new_list;
    char s1[20];
    char s2[20];

    sprintf(s1, "%d", new_ptr->value);
    new_ptr = new_ptr->next;

    while(new_ptr != NULL) {
        sprintf(s2, "%d", new_ptr->value);
        strcat(s1, s2);
        new_ptr = new_ptr->next;
    }
    return atoi(s1);
}

Node *logicAND(GateLL *root, Node *temp_values, Node *input_values) {
    int val1 = verifyVal1(root, temp_values, input_values);
    int val2 = verifyVal2(root, temp_values, input_values);
   
    int logicValue = 0;

    if(val1 == val2) {
        logicValue = val1;
    } else {
        logicValue = 0;
    }

    

    return insert(root->node->params->next->next->string, temp_values, logicValue);
    
}

Node *logicOR(GateLL *root, Node *temp_values, Node *input_values) {
    int val1 = verifyVal1(root, temp_values, input_values);
    int val2 = verifyVal2(root, temp_values, input_values);

    
    int logicValue = 0;

    if(val1 == val2) {
        logicValue = val1;
    } else {
        logicValue = 1;
    }

    return insert(root->node->params->next->next->string, temp_values, logicValue);
}

Node *logicNAND(GateLL *root, Node *temp_values, Node *input_values) {
    int val1 = verifyVal1(root, temp_values, input_values);
    int val2 = verifyVal2(root, temp_values, input_values);
    
    int logicValue = 0;

    if(val1 == val2 && val1 == 1) {
        logicValue = 0;
    } else if(val1 == val2 && val1 == 0) {
        logicValue = 1;
    } else {
        logicValue = 1;
    }

    return insert(root->node->params->next->next->string, temp_values, logicValue);
}

Node *logicNOR(GateLL *root, Node *temp_values, Node *input_values) {
    int val1 = verifyVal1(root, temp_values, input_values);
    int val2 = verifyVal2(root, temp_values, input_values);
    int logicValue = 0;

    if((val1 == val2) && (val1 == 1)) {
        logicValue = 0;
    } else if((val1 == val2) && (val1 == 0)) {
        logicValue = 1;
    } else {
        logicValue = 0;
    }

    return insert(root->node->params->next->next->string, temp_values, logicValue);
}

Node *logicXOR(GateLL *root, Node *temp_values, Node *input_values) {
    int val1 = verifyVal1(root, temp_values, input_values);
    int val2 = verifyVal2(root, temp_values, input_values);

    int logicValue = 0;

    if(val1 == val2) {
        logicValue = 0;
    } else {
        logicValue = 1;
    }

    return insert(root->node->params->next->next->string, temp_values, logicValue);
}

Node *logicNOT(GateLL *root, Node *temp_values, Node *input_values) {
    int val1 = verifyVal1(root, temp_values, input_values);
    int logicValue = 0;
    if(val1 == 1) {
        logicValue = 0;
    } else {
        logicValue = 1;
    }

    return insert(root->node->params->next->string, temp_values, logicValue);
}

Node *logicDECODER(GateLL *root, Node *temp_values, Node *input_vales) {
    int binary = LLToInteger(input_vales, root, temp_values);
    int value = IntegerConversionInput(binary);

    int i;
    Node *pointer = root->node->params;
    for(i = 0; i < root->node->size; i++) {
        pointer = pointer->next;
    }

    while(pointer != NULL) {
        if(value == 0) {
            temp_values = insert(pointer->string, temp_values, 1);
            value--;
            pointer = pointer->next;
        } else {
            temp_values = insert(pointer->string, temp_values, 0);
            value--;
            pointer = pointer->next;
        }
    }
    return temp_values;



}

Node *logicMULTIPLEXER(GateLL *root, Node *temp_values, Node *input_values) {
    int sizeOfInputs = pow(2, root->node->size);
    int sizeOfSelectors = root->node->size;
    int *arr = malloc(sizeof(int) * sizeOfInputs);
    int i, k;

    Node *ptr = root->node->params;
    for(i = 0; i < sizeOfInputs; i++) {
        arr[i] = verifyMUXValue(temp_values, input_values, ptr);
        ptr = ptr->next;
    } 

    for(k = i; k < i + sizeOfSelectors; k++) {
        ptr = ptr->next;
    }

    int binary = LLToInteger(input_values, root, temp_values);
    
    int value = IntegerConversionInput(binary);

    return insert(ptr->string, temp_values, arr[value]);
}

Node *logicPASS(GateLL *root, Node *temp_values, Node* input_values) {
     int value = verifyVal1(root, temp_values, input_values);

     return insert(root->node->params->next->string, temp_values, value);
}





int main(int argc, char **argv)
{
    // opens the file to be read
    FILE  *file = fopen(argv[1], "r");


    // used to hold the variable names after being read
    Node *input_names = NULL;
    
    char *var_name = malloc(sizeof(char) * 17);

    // skip INPUT
    fscanf(file, " %16s", var_name);

    // number of inputs, outputs should be 2^N if using special decoder/etc
    
    int input_count = 0;
    fscanf(file, "%d", &input_count);
    
    // reads the input
    int i;
    for(i = 0; i < input_count; i++) {
        fscanf(file, " %16s", var_name);
        input_names = insert(var_name, input_names, -1);
    }

    /*
    // test code to read input 
    Node *test = input_names;
    while(test != NULL) {
        printf("input_name: %16s\n", test->string);
        test = test->next;
    }

    printf("\n");*/
                              

    Node *output_names = NULL;
    int var_count = 0;
    // skip OUTPUT
    fscanf(file, " %16s", var_name);

    // number of outputs
    fscanf(file, "%d", &var_count);


    // reads output
    for(i = 0; i < var_count; i++) {
        ;
        fscanf(file, " %16s", var_name);
        output_names = insert(var_name, output_names, -1);
    }    

    /*
    //test code to read ouput 
    Node *test = output_names;
    while(test != NULL) {
        printf("input_name: %16s\n", test->string);
        test = test->next;
    }

    printf("\n");*/
    // end code for test outputs  


    // creating the linkedList of gates
    GateLL *parsed_list = NULL;

    // parsing the rest of the file in one straight SHOT
    while(fscanf(file, " %16s", var_name) != EOF) {
        if(strcmp(var_name, "AND") == 0) {
            parsed_list = insertGate(parsed_list, AND, 3, file);
        } else if(strcmp(var_name, "OR") == 0) {
            parsed_list = insertGate(parsed_list, OR, 3, file);
        } else if(strcmp(var_name, "NAND") == 0) {
            parsed_list = insertGate(parsed_list, NAND, 3, file);
        } else if(strcmp(var_name, "NOR") == 0) {
            parsed_list = insertGate(parsed_list, NOR, 3, file);
        } else if(strcmp(var_name, "XOR") == 0) {
            parsed_list = insertGate(parsed_list, XOR, 3, file);
        } else if(strcmp(var_name, "NOT") == 0) {
            parsed_list = insertGate(parsed_list, NOT, 2, file);
        } else if(strcmp(var_name, "DECODER") == 0) {
            parsed_list = insertDecoderGate(parsed_list, DECODER, file);
        } else if(strcmp(var_name, "PASS") == 0) {
            parsed_list = insertGate(parsed_list, PASS, 2, file);
        } else if(strcmp(var_name, "MULTIPLEXER") == 0) {
            parsed_list = insertMultiplexerGate(parsed_list, MULTIPLEXER, file);
        }
    }

    /*
    // test code to run parsed_list 
    GateLL *pointer = parsed_list;
    while(pointer != NULL) {
        Node *cap = pointer->node->params;
        while(cap != NULL) {
            printf("string: %16s || value: %d\n", cap->string, cap->value);
            cap = cap->next;
        }
        printf("++++++++++++++++++++++\n");
        pointer = pointer->next;
    }
     //end code */


    //printf("-----------------------------------------------------\n"); 
    
    int loop_iteration;
    Node *var_temps;
    for(loop_iteration = 0; loop_iteration < pow(2, input_count); loop_iteration++) {
        input_names = binaryConversionInput(input_count, input_names, loop_iteration);
        parsed_list = updateValue(input_names, parsed_list);
        
        
        Node *ptr = input_names;
        while(ptr != NULL) {
            printf("%d", ptr->value);
            printf(" ");
            ptr = ptr->next;
        }

        printf("| ");
        
        
        var_temps = NULL;
        GateLL *parsed_pointer = parsed_list;
        while(parsed_pointer != NULL) {
            switch(parsed_pointer->node->kind) {
                case AND:
                    var_temps = logicAND(parsed_pointer, var_temps, input_names);
                    parsed_pointer = parsed_pointer->next;
                    break;
                case OR:
                    var_temps = logicOR(parsed_pointer, var_temps, input_names);
                    parsed_pointer = parsed_pointer->next;
                    break;
                case NAND:
                    var_temps = logicNAND(parsed_pointer, var_temps, input_names);
                    parsed_pointer = parsed_pointer->next;
                    break;
                case NOR:
                    var_temps = logicNOR(parsed_pointer, var_temps, input_names);
                    parsed_pointer = parsed_pointer->next;
                    break;
                case XOR:
                    var_temps = logicXOR(parsed_pointer, var_temps, input_names);
                    parsed_pointer = parsed_pointer->next;
                    break;
                case NOT:
                    var_temps = logicNOT(parsed_pointer, var_temps, input_names);
                    parsed_pointer = parsed_pointer->next;
                    break;
                case PASS:
                    var_temps = logicPASS(parsed_pointer, var_temps, input_names);
                    parsed_pointer = parsed_pointer->next;
                    break;
                case DECODER:
                    var_temps = logicDECODER(parsed_pointer, var_temps, input_names);
                    parsed_pointer = parsed_pointer->next;
                    break;
                case MULTIPLEXER:
                    var_temps = logicMULTIPLEXER(parsed_pointer, var_temps, input_names);
                    parsed_pointer = parsed_pointer->next;
                    break;
            }
        }

        
        output_names = updateOutput(var_temps, output_names);
        ptr = output_names;
        while(ptr != NULL) {
            printf("%d", ptr->value);
            if(ptr->next != NULL) printf(" ");
            ptr = ptr->next;
        }

        printf("\n");
        

    }

    
    
    
}











