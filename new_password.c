#include <ctype.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#define MAX_PWD_SIZE 20

int is_digit(char c) {
  if (c >= '0' && c <= '9') {
    return 1;
  }
  return 0;
}

int is_lower_case(char c) {
  if (c >= 'a' && c <= 'z') {
    return 1;
  }
  return 0;
}

int is_upper_case(char c) {
  if (c >= 'A' && c <= 'Z') {
    return 1;
  }
  return 0;
}

int is_special_char(char c) {
  if (c == '.' || c == '!' || c == '@' || c == '#' || c == '$' || c == '%' ||
      c == '^' || c == '&' || c == '*' || c == '-' || c == '_') {
    return 1;
  }
  return 0;
}

void print_failed_message(int n) {
  switch (n) {
  case 1:
    printf("First attempt failed.\n");
    break;
  case 2:
    printf("Second attempt failed.\n");
    break;
  case 3:
    printf("Third attempt failed.\n");
    break;
  case 4:
    printf("Fourth attempt failed.\n");
    break;
  default:
    printf("Attempt failed.\n");
  }
}

char *change_password_prompt(int n) {
  char *new_password = (char *)malloc(MAX_PWD_SIZE * sizeof(char));

  printf("Enter your new password (");

  if (n % 10 == 1) {
    printf("%dst attempt): ", n);
  } else if (n % 10 == 2) {
    printf("%dnd attempt): ", n);
  } else if (n % 10 == 3) {
    printf("%drd attempt): ", n);
  } else {
    printf("%dth attempt): ", n);
  }

  scanf("%s", new_password);

  return new_password;
}

char *convert_to_lowercase(const char *str) {
  char *lowercase_str = strdup(str);
  for (int i = 0; lowercase_str[i]; i++) {
    lowercase_str[i] = tolower(lowercase_str[i]);
  }
  return lowercase_str;
}

char *strip_hyphens(char *dob) {
  char *dob_stripped = (char *)malloc((strlen(dob) - 2) * sizeof(char));
  for (int i = 0, j = 0; i < strlen(dob); i++) {
    if (dob[i] == '-') {
      continue;
    } else {
      dob_stripped[j++] = dob[i];
    }
  }

  return dob_stripped;
}

int match_substrings(char *str1, char *str2, int min_w) {
  int max = 0;
  int w = min_w;
  int len1 = strlen(str1);
  int len2 = strlen(str2);
  for (; w <= len1; w++) {
    for (int i = 0; i <= len1 - w; i++) {
      char substr[w + 1];
      strncpy(substr, str1 + i, w);
      substr[w] = '\0';

      if (strstr(str2, substr) != NULL) {
        max = max > w ? max : w;
      }
    }
  }

  return max;
}

int validate_new_password(char *username, char *new_password,
                          char *password_file) {
  int is_valid_password = 1;
  int password_length = strlen(new_password);

  FILE *pwd_ptr = fopen(password_file, "r");
  if (pwd_ptr == NULL) {
    printf("Error opening %s.\n", password_file);
    exit(1);
  }
  FILE *master_ptr = fopen("masterfile.txt", "r");
  if (master_ptr == NULL) {
    printf("Error opening masterfile.txt.\n");
    exit(1);
  }

  // R1
  if (password_length < 12) {
    printf("Password does not contain a minimum of 12 characters.\n");
    is_valid_password = 0;
  }

  // R2
  int contains_upper_case = 0;
  for (int i = 0; i < password_length; i++) {
    if (is_upper_case(new_password[i])) {
      contains_upper_case = 1;
      break;
    }
  }
  if (!contains_upper_case) {
    printf("Password does not contain at least one uppercase letter.\n");
    is_valid_password = 0;
  }

  // R3
  int contains_lower_case = 0;
  for (int i = 0; i < password_length; i++) {
    if (is_lower_case(new_password[i])) {
      contains_lower_case = 1;
      break;
    }
  }
  if (!contains_lower_case) {
    printf("Password does not contain at least one lowercase letter.\n");
    is_valid_password = 0;
  }

  // R4
  int contains_digit = 0;
  for (int i = 0; i < password_length; i++) {
    if (is_digit(new_password[i])) {
      contains_digit = 1;
      break;
    }
  }
  if (!contains_digit) {
    printf("Password does not contain at least one digit.\n");
    is_valid_password = 0;
  }

  // R5
  int contains_special_char = 0;
  for (int i = 0; i < password_length; i++) {
    if (is_special_char(new_password[i])) {
      contains_special_char = 1;
      break;
    }
    if (!is_digit(new_password[i]) && !is_lower_case(new_password[i]) &&
        !is_upper_case(new_password[i])) {
      break;
    }
  }
  if (!contains_special_char) {
    printf("Password does not contain at least one of the allowed special "
           "characters.\n");
    is_valid_password = 0;
  }

  // R6
  char old_password[MAX_PWD_SIZE];
  char *new_password_lowercase = convert_to_lowercase(new_password);
  int chars_matched = 0;
  while (fscanf(pwd_ptr, "%s", old_password) != EOF) {
    char *old_password_lowercase = convert_to_lowercase(old_password);

    chars_matched =
        match_substrings(new_password_lowercase, old_password_lowercase, 4);
    free(old_password_lowercase);
    if (chars_matched >= 4) {
      printf("Password contains %d characters consecutively similar to one of "
             "the past 10 passwords.\n",
             chars_matched);
      is_valid_password = 0;
      break;
    }
  }

  // R7 and R8
  char user[50];
  char dob[15];
  char pwd_file[100];
  while (fscanf(master_ptr, "%s %s %s", user, dob, pwd_file) != EOF) {
    if (strcmp(user, username) != 0) {
      continue;
    }

    char *username_copy = strdup(username);
    char name[50];
    int i = 0, j = 0;
    while (username_copy[i] != '.') {
      name[i] = username_copy[i];
      i++;
    }
    name[i] = '\0';
    i++;
    char *lowercase_name = convert_to_lowercase(name);

    char surname[50];
    while (i != strlen(username_copy)) {
      surname[j] = username_copy[i];
      i++;
      j++;
    }
    surname[j] = '\0';
    char *lowercase_surname = convert_to_lowercase(surname);

    int name_is_in_password =
        (strstr(new_password_lowercase, lowercase_name) != NULL);
    int surname_is_in_password =
        (strstr(new_password_lowercase, lowercase_surname) != NULL);

    if (name_is_in_password && surname_is_in_password) {
      printf("Password contains both the name and surname portions of the "
             "username.\n");
      is_valid_password = 0;
    } else if (name_is_in_password) {
      printf("Password contains the name portion of the username.\n");
      is_valid_password = 0;
    } else if (surname_is_in_password) {
      printf("Password contains the surname portion of the username.\n");
      is_valid_password = 0;
    }
    free(lowercase_surname);
    free(lowercase_name);
    free(username_copy);

    char *dob_stripped = strip_hyphens(dob);

    chars_matched = match_substrings(new_password_lowercase, dob_stripped, 4);

    if (chars_matched >= 3) {
      printf("Password contains %d digits consecutively similar to the date "
             "of birth.\n",
             chars_matched);
      is_valid_password = 0;
    }

    free(dob_stripped);
    break;
  }

  free(new_password_lowercase);
  fclose(pwd_ptr);
  fclose(master_ptr);

  return is_valid_password;
}

void replace_password(char *password_file, char *new_password) {
  FILE *pwd_ptr = fopen(password_file, "r");
  fseek(pwd_ptr, 0, SEEK_END);
  long file_size = ftell(pwd_ptr);
  fseek(pwd_ptr, 0, SEEK_SET);
  char *file_contents = (char *)malloc((file_size + 1) * sizeof(char));
  fread(file_contents, 1, file_size, pwd_ptr);
  fclose(pwd_ptr);

  int no_of_lines = 0;
  for (int i = 0; i < file_size; i++) {
    if (file_contents[i] == '\n') {
      no_of_lines++;
    }
  }

  char *new_file_contents;

  if (no_of_lines == 10) {
    int file_size_without_last_line = file_size - 2;

    while (file_contents[file_size_without_last_line] != '\n') {
      file_size_without_last_line--;
    }
    file_size_without_last_line++;

    new_file_contents = (char *)malloc(
        (file_size_without_last_line + strlen(new_password) + 2) *
        sizeof(char));
    strncpy(new_file_contents, new_password, strlen(new_password));
    new_file_contents[strlen(new_password)] = '\n';
    strncpy(new_file_contents + strlen(new_password) + 1, file_contents,
            file_size_without_last_line);
    new_file_contents[file_size_without_last_line + strlen(new_password) + 1] =
        '\0';
  } else {
    new_file_contents =
        (char *)malloc((file_size + strlen(new_password) + 2) * sizeof(char));
    strncpy(new_file_contents, new_password, strlen(new_password));
    new_file_contents[strlen(new_password)] = '\n';
    strncpy(new_file_contents + strlen(new_password) + 1, file_contents,
            file_size);
    new_file_contents[file_size + strlen(new_password) + 1] = '\0';
  }

  FILE *pwd_ptr_write = fopen(password_file, "w");
  fputs(new_file_contents, pwd_ptr_write);
  free(file_contents);
  free(new_file_contents);
  fclose(pwd_ptr_write);
}

void change_password(char *username, char *password_file) {
  char *new_password;

  int done = 0;
  int attempt = 1;
  while (attempt <= 4) {
    new_password = change_password_prompt(attempt);

    done = validate_new_password(username, new_password, password_file);

    if (done) {
      replace_password(password_file, new_password);
      break;
    }
    print_failed_message(attempt);
    for (int i = 8 * (int)pow(2, attempt - 1); i > 0 && attempt != 4; i--) {
      printf("Wait for %d seconds\n", i);
      sleep(1);
    }
    attempt++;
    free(new_password);
    if (attempt == 5) {
      printf("All 4 attempts failed. You need to try again later.\n");
      exit(1);
    }
  }
}
