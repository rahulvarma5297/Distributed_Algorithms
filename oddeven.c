#include <time.h>
#include <math.h>
#include <stdio.h>
#include <limits.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdbool.h>
#include <sys/time.h>

typedef struct process
{
    int id;
    int value;
} *PROCESS;

typedef struct event
{
    int Source;
    int Target;
    int Type;
    int Flag;
    int Value;
    struct event *next_ptr;
} *EVENT;

void print(PROCESS sequence, int n)
{
    for (int i = 0; i < n; i++)
    {
        printf("%d", sequence[i].value);
        printf("\t");
    }
}

EVENT event_initialization(EVENT event)
{
    event = (EVENT)malloc(sizeof(struct event));
    event->next_ptr = NULL;
    return event;
}

PROCESS generating_the_value_process(PROCESS sequence, int number_process, int seq_type)
{
    int n = number_process;
    sequence = (PROCESS)malloc(number_process * sizeof(struct process));
    if (seq_type == 1)
    {
        printf("\n");
        printf("SORTED SEQUENCE GENERATED");
        printf("\n");
        for (int i = 0; i < n; i++)
        {
            int value = i + 1;
            sequence[i].id = i;
            sequence[i].value = value;
        }
    }
    else if (seq_type == 2)
    {
        srand(time(0));
        printf("\n");
        printf("RANDOM SEQUENCE GENERATED");
        printf("\n");
        for (int i = 0; i < n; i++)
        {
            int value = rand() / 1000;
            sequence[i].id = i;
            sequence[i].value = value;
        }
    }
    else if (seq_type == 3)
    {
        printf("\n");
        printf("REVERSELY SORTED SEQUENCE GENERATED");
        printf("\n");

        for (int i = 0; i < n; i++)
        {
            int value = n - i;
            sequence[i].id = i;
            sequence[i].value = value;
        }
    }
    else
    {
        printf("\n");
        printf("Please Enter the sequence you want to sort!");
        printf("\n");

        for (int i = 0; i < n; i++)
        {
            int value;
            printf("\t");
            printf("Enter the value of node %d: ", i + 1);
            scanf("%d", &value);
            sequence[i].id = i;
            sequence[i].value = value;
        }
    }

    return sequence;
}

int isempty(EVENT event)
{
    if (event->next_ptr != NULL)
    {
        return 0;
    }
    return 1;
}

int swap(int *a, int *b)
{
    int minimum, temp = 0;
    if (*a > *b)
    {
        minimum = *b;
        *b = *a;
        *a = minimum;
        temp = 1;
    }
    else
    {
        minimum = *a;
    }
    return temp;
}

EVENT event_insert(EVENT event, int source_event, int target_event, int event_type, int type, int event_value)
{
    EVENT temp, previous = event, current;
    temp = (EVENT)malloc(sizeof(struct event));

    temp->Source = source_event;
    temp->Target = target_event;
    temp->Type = event_type;
    temp->Flag = type;
    temp->Value = event_value;
    temp->next_ptr = NULL;

    if (previous->next_ptr != NULL)
    {
        current = previous;
        while (current->next_ptr != NULL)
        {
            current = current->next_ptr;
        }
        current->next_ptr = temp;
    }
    else
    {
        previous->next_ptr = temp;
    }
    return event;
}

EVENT find_top_event(EVENT event, EVENT top)
{
    EVENT del = event, next, temp = top;
    temp = temp->next_ptr;

    if (del->next_ptr != NULL)
    {
        del = del->next_ptr;
        if (del->next_ptr == NULL)
        {
            event->next_ptr = NULL;
        }
        else
        {
            next = del->next_ptr;
            event->next_ptr = next;
        }

        temp->Source = del->Source;
        temp->Target = del->Target;
        temp->Type = del->Type;
        temp->Flag = del->Flag;
        temp->Value = del->Value;
        temp->next_ptr = NULL;
        free(del);
    }
    else
    {
        top->next_ptr = NULL;
        temp->next_ptr = NULL;
    }
    return top;
}

EVENT deleting_events(EVENT event)
{
    EVENT del = event, next;
    if (del->next_ptr != NULL)
    {
        del = del->next_ptr;
        while (del->next_ptr != NULL)
        {
            next = del;
            free(next);
            del = del->next_ptr;
        }
        free(del);
        event->next_ptr == NULL;
    }
    else
    {
        event->next_ptr = NULL;
    }
    return event;
}

void send_event(EVENT event, EVENT current)
{
    EVENT temp = current->next_ptr;
    event = event_insert(event, temp->Target, temp->Source, 0, temp->Flag, temp->Value);
}

void recieve_event(PROCESS sequence, EVENT event, EVENT current, int round)
{
    EVENT temp = current->next_ptr;
    int flag = temp->Flag, rcv_val = temp->Value;

    if (flag)
    {
        bool s = swap(&rcv_val, &sequence[temp->Source].value);
        event = event_insert(event, temp->Source, temp->Target, 1, 0, rcv_val);
    }
    else
    {
        sequence[temp->Source].value = rcv_val;
    }
}

int main()
{
    clock_t start, end;
    double cpu_time_used;
    // Write code for calculating memory usage
    size_t total_memory = 0;

    int round = 1;
    PROCESS sequence = NULL;
    EVENT event, top_event, top_nxt;
    srand(1000);
    int n;
    printf("Enter the Number of Process: ");
    scanf("%d", &n);
    printf("\n");

    event = event_initialization(event);
    top_event = event_initialization(top_event);
    top_nxt = event_initialization(top_nxt);
    top_event->next_ptr = top_nxt;

    int choice;
    printf("1. BEST CASE (SORTED)");
    printf("\n");
    printf("2. RANDOM SEQUENCE");
    printf("\n");
    printf("3. WORST CASE (REVERSELY SORTED)");
    printf("\n");
    printf("4. USER INPUT");
    printf("\n\n");

    printf("Choose the sequence you want to sort: ");
    scanf("%d", &choice);

    sequence = generating_the_value_process(sequence, n, choice);

    printf("\n");
    printf("The Generated Sequence is : ");

    for (int i = 0; i < n; i++)
    {
        printf("%d", sequence[i].value);
        printf("   ");
    }
    printf("\n\n");
    start = clock();
    while (round <= n)
    {
        printf("\n");
        printf("ROUND - ");
        printf("%d", round);
        printf("\t");

        for (int i = 0; i < n - 1; i++)
        {
            if (i % 2 != round % 2)
            {
                event = event_insert(event, i, i + 1, 1, 1, sequence[i].value);
            }
        }

        while (!isempty(event))
        {
            top_event = find_top_event(event, top_event);

            if (top_event->next_ptr->Type)
            {
                send_event(event, top_event);
            }
            else
            {
                if (!top_event->next_ptr->Type)
                {
                    recieve_event(sequence, event, top_event, round);
                }
            }
        }
        for (int i = 0; i < n; i++)
        {
            printf("%d", sequence[i].value);
            printf("  ");
        }
        round++;
    }

    printf("\n");
    printf("\n");
    printf("Final Output : ");
    print(sequence, n);

    end = clock();
    cpu_time_used = ((double)(end - start)) / CLOCKS_PER_SEC;

    printf("\n");
    printf("Time Taken for Execution :");
    printf("%f", cpu_time_used);
    printf(" seconds");
    printf("\n");

    // Time Complxity Included in the Report File
    // TC -> O(n ^ 2);
    // SC -> O(n);

    event = deleting_events(event);
    top_event = deleting_events(top_event);

    free(sequence);
    free(event);
    free(top_event);

    printf("\n");

    return 0;
}