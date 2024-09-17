#include "new_password.c"
#include <stdio.h>
#include <string.h>

int main() {
  char username[50];
  char entered_password[20];
  int count = 0;

  printf("Enter username: ");
  scanf("%s", username);

  FILE *ptr = fopen("masterfile.txt", "r");
  char m_username[50];
  char password_file_name[100];
  char date[50];

  int valid_username = 0;

  while (fscanf(ptr, "%s %s %s", m_username, date, password_file_name) != EOF) {
    if (strcmp(username, m_username) == 0) {
      valid_username = 1;
      break;
    }
  }

  if (!valid_username) {
    printf("Invalid username entered");
    return 1;
  }

  char latest_password[20];
  FILE *p = fopen(password_file_name, "r");
  fscanf(p, "%s", latest_password);
  printf("Enter password: ");
  while (count < 3) {
    scanf("%s", entered_password);
    if (strcmp(latest_password, entered_password) == 0) {
      change_password(username, password_file_name);
      break;
    }
    printf("Wrong password! Enter password again:");
    count++;
  }

  if (count == 3) {
    printf("Wrong password entered 3 times. Application exiting...");
    return 1;
  }

  return 0;
}
