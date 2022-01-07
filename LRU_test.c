#include <stdio.h>
#include <string.h>

#define FIFO 0
#define LRU 1
#define SWAPPING LRU
#define MAXPAGES 3
char test[500] = "7,0,1,2,0,3,0,4,2,3,0,3,0,3,2,1,2,0,1,7,0,1";

int faults = 0;
int misses = 0;
int hits = 0;
int total_pages = 0;

#if (SWAPPING==LRU)
struct page_table {
    int page_num;
    struct page_table* next;
    struct page_table* prev;
};
#endif

#define NIL_PAGE ((struct page_table *) 0)

struct page_table* head = NIL_PAGE;
struct page_table* tail = NIL_PAGE;

void print_all_pages_in_memory() {
    if (head == NIL_PAGE) {
        printf("no pages in memory");
    }
    else {
        struct page_table* page = head;
        printf("pages:\n");
        while (page != NIL_PAGE) {
            printf("%d \n", page->page_num);
            page = page->next;
        }
        printf("--------------------\n");

    }
}

void add_page(struct page_table* page) {

    if (head == NIL_PAGE) {

        misses++;
        page->next = NIL_PAGE;
        page->prev = NIL_PAGE;
        head = page;
        tail = page;
        total_pages++;
    }
    else if (page->next != NIL_PAGE && page->prev != NIL_PAGE) {
        hits++;
        printf("HIT : page number %d is already in memory\n", page->page_num);
        page->prev->next = page->next;
        page->next->prev = page->prev;
        page->next = head;
        page->prev = NIL_PAGE;
        head->prev = page;
        head = page;
    }
    else if (head == page) {
        hits++;
        printf("HIT : page number %d is already in memory\n", page->page_num);
    }
    else if (tail == page) {
        hits++;
        printf("HIT : page number %d is already in memory\n", page->page_num);
        tail = page->prev;
        tail->next = NIL_PAGE;
        page->next = head;
        page->prev = NIL_PAGE;
        head->prev = page;
        head = page;
    }
    else if (total_pages == MAXPAGES) {
        misses++;
        faults++;

        struct page_table* p = tail;
        page->next = head;
        page->prev = NIL_PAGE;
        head->prev = page;
        head = page;

        tail = tail->prev;
        tail->next = NIL_PAGE;

        p->next = NIL_PAGE;
        p->prev = NIL_PAGE;
    }
    else {
        misses++;
        total_pages++;
        page->next = head;
        page->prev = NIL_PAGE;
        head->prev = page;
        head = page;
    }
}
void problem() {
    printf("Test case : \n%s\n", test);
}

void initialize_pages(struct page_table* page) {

    for (int i = 0; i < 8; i++) {
        struct page_table p;
        p.next = NIL_PAGE;
        p.page_num = i;
        page[i] = p;
    }
}
int main() {

    struct page_table p1[8];
    initialize_pages(p1);
    for (int i = 0; i < strlen(test); i += 2) {
        int page = test[i] - '0';
        printf("page number %d entered\n", page);
        add_page(&p1[page]);
        print_all_pages_in_memory();
    }

    printf("Hits : %d , Misses: %d ,Faults: %d", hits, misses, faults);

    return 0;
}