#include "string.h"

#include <stdio.h>

coco::String createString() {
    char stack_string[] = "Hello World!";
    coco::String string(stack_string);
    printf("this is the string i created: %s\n", string.c_str());
    return string;
}

int main() {
    coco::String string = createString();
    const char* tranformed_string = string.c_str();

    printf("%s\n", tranformed_string);
    printf("The string has the size of: %d\n", string.size());

    char first_char = string.get(0);
    char last_char = string.get(string.size() - 1);
    printf("First character: %c\n", first_char);
    printf("Last character: %c\n", last_char);

//    string.modify(0, 'L');
//    printf("Modified string: %s\n", string.c_str());
}

