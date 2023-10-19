// NoC simulator in C
// Em duplas desenvolver um ”simulador”(em software) de um NoC em anel com N nodos
// Pedir, uma única vez, para o usuário a quantidade de nodos, com limite de 10 nodos
// Sempre (while(1)) pedir:
// Quem é o nodo Source?
// Quem é o nodo Target?
// Deve-se gerar o log das operações simuladas
// Ex:
// Quem é o Source? 0
// Quem é o Target? 1

// Proc[0] criou a mensagem
// Proc[0] enviou a mensagem para o Proc[1]
// Proc[1] recebeu a mensagem de Proc[0]
// Proc[1] é o destino
// Proc[1] consumiu a mensagem
// Algoritmo de arbitragem decide se o pacote é para ser consumido ou repassado adiante
// Algoritmo de roteamento decide para qual lado enviar (direita, esquerda ou o menor caminho)

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

    for(int i = 0; i < numNodes; i++){
        if(i == 0) {
            nodes[i] = createNode(i, NULL, NULL);
        } else if(i == numNodes - 1) {
            nodes[i] = createNode(i, nodes[i-1], nodes[0]);
            nodes[0]->left = nodes[i];
        } else {
            nodes[i] = createNode(i, nodes[i-1], NULL);   
            nodes[i-1]->right = nodes[i];     
        }

        printf("Node %d criado\n", nodes[i]->id);
    }

    return nodes;
}


int main(){

    int numNodes;

    printf("Digite o numero de nodos: ");
    scanf("%d", &numNodes);

    if(numNodes > MAX_NODES){
        printf("Numero de nodos invalido!\n");
        return 0;
    }

    Node** nodes = createNodes(numNodes);

    int source, target;
    printf("Quem é o source? ");
    scanf("%d", &source);

    printf("Quem é o target? ");
    scanf("%d", &target);

    Message* message = (Message*) malloc(sizeof(Message));
    message->id = 0;
    message->source = source;
    message->target = target;
    message->message = 0;
    message->consumed = 0;

    nodes[source]->message = message;

    

    while(1){
        // Check if there is a message at the current node
        if(nodes[source]->message != NULL) {
            Message* message = nodes[source]->message;
            printf("Proc[%d] criou a mensagem\n", message->source);

            // Send the message to the next node
            Node* nextNode = nodes[source]->right;
            nextNode->message = message;
            printf("Proc[%d] enviou a mensagem para o Proc[%d]\n", message->source, nextNode->id);

            // Receive the message at the target node
            while(nextNode->id != target) {
                nextNode = nextNode->right;
                printf("Proc[%d] repassou a mensagem para o Proc[%d]\n", nextNode->left->id, nextNode->id);
            }
            printf("SAS");

            printf("Proc[%d] recebeu a mensagem de Proc[%d]\n", nextNode->id, nextNode->left->id);

            // Consume the message at the target node
            nextNode->message->consumed = 1;
            printf("Proc[%d] consumiu a mensagem\n", nextNode->id);

            // Arbitration algorithm (always consume the message)
            if(nextNode->message->consumed) {
                printf("Algoritmo de arbitragem decidiu consumir a mensagem\n");
            } else {
                printf("Algoritmo de arbitragem decidiu repassar a mensagem\n");
            }

            // Routing algorithm (always send the message to the right)
            nextNode->right->message = nextNode->message;
            printf("Algoritmo de roteamento decidiu enviar a mensagem para o Proc[%d]\n", nextNode->right->id);

            // Clear the message at the current node
            nodes[source]->message = NULL;

        }

    }

    return 0;


}

