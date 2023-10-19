#include <stdio.h>
#include <stdlib.h>

#define MAX_NODES 10

typedef struct Message {
    int id;
    int source;
    int target;
    int message;
    int consumed;
} Message;

typedef struct Node {
    int id;
    struct Node* left;
    struct Node* right;
    Message* message;    
} Node;

Node* createNode(int id, Node* left, Node* right) {
    Node* node = (Node*) malloc(sizeof(Node));
    node->id = id;
    node->left = left;
    node->right = right;
    node->message = NULL;
    return node;
}

Node** createNodes(int numNodes) {
    Node** nodes = (Node**) malloc(numNodes * sizeof(Node*));

    for(int i = 0; i < numNodes; i++) {
        nodes[i] = createNode(i, NULL, NULL);
    }

    for(int i = 0; i < numNodes; i++) {
        nodes[i]->left = nodes[(i - 1 + numNodes) % numNodes];
        nodes[i]->right = nodes[(i + 1) % numNodes];
    }

    return nodes;
}

void simulateMessagePassing(Node** nodes, int source, int target) {
    Message* message = (Message*) malloc(sizeof(Message));
    message->id = 0;
    message->source = source;
    message->target = target;
    message->message = 0;
    message->consumed = 0;

    nodes[source]->message = message;

    Node* currentNode = nodes[source];
    
    printf("Proc[%d] criou a mensagem\n", message->source);

    while(1){
        if(nodes[source]->message != NULL) {
            if(message->target == currentNode->id) {
                printf("Proc[%d] é o destino\n", currentNode->id);
                printf("Proc[%d] consumiu a mensagem\n", currentNode->id);
                nodes[source]->message->consumed = 1;
                break;
            } else {
                printf("Proc[%d] enviou a mensagem para o Proc[%d]\n", currentNode->id, currentNode->right->id);
                printf("Proc[%d] recebeu a mensagem de Proc[%d]\n", currentNode->right->id, currentNode->id);
                currentNode->right->message = currentNode->message;
                currentNode = currentNode->right;
            }
        }
    }

    free(message);
}

int main(){
    int numNodes;

    printf("Digite o numero de nodos: ");

    while (scanf("%d", &numNodes) && numNodes < 2 || numNodes > MAX_NODES){
        if (numNodes < 2 || numNodes > MAX_NODES){
            printf("Numero de nodos invalido\nDigite novamente: ");
        }
    }

    Node** nodes = createNodes(numNodes);

    int source, target;

    printf("Quem é o source? ");

    while (scanf("%d", &source) && source >= numNodes || source < 0){
        if (source < 0 || source >= numNodes){
            printf("Source invalido\nDigite novamente: ");
        }
    }

    printf("Quem é o target? ");

    while (scanf("%d", &target) && target >= numNodes || target < 0){
        if (target < 0 || target >= numNodes){
            printf("Target invalido\nDigite novamente: ");
        }
    }

    simulateMessagePassing(nodes, source, target);

    free(nodes);

    return 0;
}