#include <stdio.h>
#include <stdlib.h>

#include <math.h>

// structure for a double ll
struct node
{
    int data;
    struct node *next;
    struct node *prev;
};
int math_min(int a, int b)
{
    if (a < b)
    {
        return a;
    }
    return b;
}
// insertion at end of DLL
void insert(struct node **head, int data)
{
    struct node *newnode = (struct node *)malloc(sizeof(struct node));
    newnode->data = data;
    newnode->next = NULL;
    newnode->prev = NULL;
    if (*head == NULL)
    {
        *head = newnode;
        return;
    }
    struct node *temp = *head;
    while (temp->next != NULL)
    {
        temp = temp->next;
    }
    temp->next = newnode;
    newnode->prev = temp;
}

void traverse(struct node *head)
{
    struct node *temp = head;
    while (temp != NULL)
    {
        printf("%d ", temp->data);
        temp = temp->next;
    }
}

void local_minima(struct node *head, int n)
{
    struct node *temp = head;
    int count = 0;
    while (temp != NULL)
    {
        count++;
        temp = temp->next;
    }
    temp = head;
    int i = 0;
    printf("\n");
    while (temp != NULL)
    {
        int flag = 0;
        struct node *temp1 = temp;
        for (int j = 0; j < math_min(n, i); j++)
        {
            temp1 = temp1->prev;
            if (temp1->data < temp->data)
            {
                flag = 1;
                break;
            }
        }
        if (flag == 0)
        {
            temp1 = temp;
            for (int j = 0; j < math_min(n, count - i-1); j++)
            {
                temp1 = temp1->next;
                if (temp1->data < temp->data)
                {
                    flag = 1;
                    break;
                }
            }
        }
        if (flag == 0)
        {
            printf("%d ", temp->data);
        }

        temp = temp->next;
        i++;
    }
}

int main()
{
    struct node *head = NULL;
    insert(&head, 10);
    insert(&head, 2);
    insert(&head, 4);
    insert(&head, 7);
    insert(&head, 6);
    insert(&head, 3);
    insert(&head, 11);
    // insert(&head, -1);
    traverse(head);
    local_minima(head, 2);
    return 0;
}