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
    int Lvalue;
    int Rvalue;
    int nValue;
    int Sliding;
    int LProcess;
    int RProcess;
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

void print_result(PROCESS sequence, int n)
{
    for (int i = 0; i < n; i++)
    {
        printf("%d", sequence[i].nValue);
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
    sequence = (PROCESS)malloc(n * sizeof(struct process));

    if (seq_type == 1)
    {
        printf("\n");
        printf("Input Generation: SORTED SEQUENCE!");
        printf("\n");
        for (int i = 0; i < n; i++)
        {
            int val = i + 1;
            sequence[i].id = i;
            sequence[i].nValue = val;
            sequence[i].Sliding = i % 3;
            sequence[i].Lvalue = -1;
            sequence[i].Rvalue = -1;
        }
    }
    else if (seq_type == 2)
    {
        printf("\n");
        printf("Input Generation: RANDOM SEQUENCE!");
        printf("\n");
        srand(time(0));
        for (int i = 0; i < n; i++)
        {
            int val = rand() / 1000;
            sequence[i].id = i;
            sequence[i].nValue = val;
            sequence[i].Sliding = i % 3;
            sequence[i].Lvalue = -1;
            sequence[i].Rvalue = -1;
        }
    }
    else if (seq_type == 3)
    {
        printf("\n");
        printf("Input Generation: WORST CASE (REVERSELY SORTED)!");
        printf("\n");
        for (int i = 0; i < n; i++)
        {
            int val = n - i;
            sequence[i].id = i;
            sequence[i].nValue = val;
            sequence[i].Sliding = i % 3;
            sequence[i].Lvalue = -1;
            sequence[i].Rvalue = -1;
        }
    }
    else
    {
        printf("\n");
        printf("\t");
        printf("Please Enter the sequence you want to sort!");
        printf("\n");

        for (int i = 0; i < n; i++)
        {
            int val;
            printf("Enter the value for node %d: ", i + 1);
            scanf("%d", &val);
            sequence[i].id = i;
            sequence[i].nValue = val;
            sequence[i].Sliding = i % 3;
            sequence[i].Lvalue = -1;
            sequence[i].Rvalue = -1;
        }
    }
    return sequence;
}

int isEmpty(EVENT event)
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

void update(PROCESS sequence, int id)
{
    sequence[id].Lvalue = -1;
    sequence[id].Rvalue = -1;
    sequence[id].Sliding = (sequence[id].Sliding + 1) % 3;
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

    if (flag == 0)
    {
        sequence[temp->Source].nValue = rcv_val;
        update(sequence, temp->Source);
    }
    else if (flag == 1)
    {
        sequence[temp->Source].Lvalue = rcv_val;
        bool s = swap(&sequence[temp->Source].Lvalue, &sequence[temp->Source].nValue);
        sequence[temp->Source].LProcess = temp->Target;
    }
    else if (flag == 2)
    {
        sequence[temp->Source].Rvalue = rcv_val;
        bool s = swap(&sequence[temp->Source].nValue, &sequence[temp->Source].Rvalue);
        sequence[temp->Source].RProcess = temp->Target;
        flag = 3;
    }
    else if (flag == 4)
    {
        sequence[temp->Source].Lvalue = rcv_val;
        bool s = swap(&sequence[temp->Source].Lvalue, &sequence[temp->Source].nValue);
        event = event_insert(event, temp->Source, temp->Target, 1, 0, sequence[temp->Source].Lvalue);
        update(sequence, temp->Source);
    }

    if (flag == 3)
    {
        if (sequence[temp->Source].Lvalue == -1)
        {
            event = event_insert(event, temp->Source, temp->Target, 1, 0, sequence[temp->Source].Rvalue);
        }
        else
        {
            bool s = swap(&sequence[temp->Source].Lvalue, &sequence[temp->Source].nValue);
            event = event_insert(event, temp->Source, sequence[temp->Source].LProcess, 1, 0, sequence[temp->Source].Lvalue);
            event = event_insert(event, temp->Source, sequence[temp->Source].RProcess, 1, 0, sequence[temp->Source].Rvalue);
        }
        update(sequence, temp->Source);
    }
}

int main()
{
    clock_t start, end;
    double cpu_time_used;

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

    printf("Choose Input Generation Method: ");
    scanf("%d", &choice);

    sequence = generating_the_value_process(sequence, n, choice);
    printf("\n");
    printf("Initial : ");
    for (int i = 0; i < n; i++)
    {
        printf("%d", sequence[i].nValue);
        printf(" (");
        printf("%d", sequence[i].Sliding);
        printf(")  ");
    }
    printf("\n");
    start = clock();
    while (round < n)
    {
        printf("\n");
        printf("ROUND - ");
        printf("%d:", round);
        printf("  ");

        for (int i = 0; i < n; i++)
        {
            if (sequence[i].Sliding == 0)
            {
                if (i < n - 2)
                {
                    event = event_insert(event, i, i + 1, 1, 1, sequence[i].nValue);
                }
                else if (i == n - 2)
                {
                    event = event_insert(event, i, i + 1, 1, 4, sequence[i].nValue);
                }
                else
                {
                    update(sequence, i);
                    continue;
                }
            }
            else if (sequence[i].Sliding == 2)
            {
                if (i != 0)
                {
                    event = event_insert(event, i, i - 1, 1, 2, sequence[i].nValue);
                }
                else
                {
                    update(sequence, i);
                    continue;
                }
            }
            else
            {
                continue;
            }
        }
        while (!isEmpty(event))
        {
            top_event = find_top_event(event, top_event);

            if (top_event->next_ptr->Type == 1)
            {
                send_event(event, top_event);
            }
            else
            {
                if (top_event->next_ptr->Type == 0)
                {
                    recieve_event(sequence, event, top_event, round);
                }
            }
        }
        for (int i = 0; i < n; i++)
        {
            printf("%d", sequence[i].nValue);
            printf(" (");
            printf("%d", sequence[i].Sliding);
            printf(")  ");
        }
        round++;
    }
    printf("\n");
    printf("\t");

    printf("\n");
    printf("SOLUTION : ");
    print_result(sequence, n);

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

    return 0;
}
