#include <mysql.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define host "localhost"
#define user "root"
#define pass ""

int main(int argc, char **argv)
{


	MYSQL *con = mysql_init(NULL), *connection;
	connection = mysql_real_connect(con, host, user, pass, "user", 0, NULL, 0);

	int choice;
	item:printf("(1) Register\n");
	printf("(2) Login\n");

	printf("shell>");
	scanf("%d", &choice);

	if(choice == 1){
		char username[30], password[30], email[30], buf[200];
		printf("Username : ");
		scanf("%s", &username);
		printf("Password : ");
		scanf("%s", &password);
		printf("E-mail : ");
		scanf("%s", &email);
		if(strlen(username) > 30){
			printf("Username cannot be greater than 30.\n");
			return 1;
		} else if(strlen(password) > 30){
			printf("Password cannot be greater than 30.\n");
			return 1;
		} else if(strlen(email) > 30){
			printf("Email cannot be greater than 30.\n");
			return 1;
		}

		sprintf(buf, "INSERT INTO users(username, password, email) VALUES ('%s', '%s', '%s')", username, password, email);
		int user_query = mysql_query(con, buf);

		if(user_query == 0){
			printf("Signup Successful.\n");
		} else {
			fprintf(stderr, "%s\n", mysql_error(con));
		}
	} else if( choice == 2){
		char username[30], username2[30], password[30], password2[30], buf[200];
		item2:printf("Username : ");
		scanf("%s", &username);
		printf("Password : ");
		scanf("%s", &password);
		if(strlen(username) > 30){
			printf("Username just can be 30 length.");
			return 1;
		} else if(strlen(password) > 30){
			printf("Password just can be 30 length.");
			return 1;
		}

		sprintf(buf, "SELECT * FROM users WHERE username = '%s' AND password = '%s'", username, password);
		int user_login_query = mysql_query(con, buf);
		MYSQL_RES *result = mysql_store_result(con);
		unsigned int num_fields = mysql_num_fields(result);
		my_ulonglong num_rows;
		num_rows = mysql_num_rows(result);

		if(num_rows > 0){
			int choice_2;
			printf("\e[1;1H\e[2J");
			printf("Welcome to your panel %s", username);
			item3:printf("\n\n(1) Change username\n");
			printf("(2) Change password\n");
			printf("(3) Add new friends\n");
			printf("(4) Show friend requests\n");
			printf("(5) Accept friend\n");
			printf("(6) Show friends\n");
			printf("(7) Delete friend\n\n");
			printf("shell>");
			scanf("%d", &choice_2);
			if(choice_2 == 1){
				char old_user_name[20], new_user_name[20], buffer[200];
				printf("Old Username : ");
				scanf("%s", &old_user_name);
				printf("New Username : ");
				scanf("%s", &new_user_name);
				if(strcmp(username, old_user_name) == 0){
					sprintf(buffer, "UPDATE users SET username = '%s' WHERE username = '%s'", new_user_name, old_user_name);
					int user_change_query = mysql_query(con, buffer);
					if(user_change_query == 0){
						printf("Username successfully changed.\n");
						goto item3;
					} else {
						fprintf(stderr, "%s\n", mysql_error(con));
					}
				}
			} else if(choice_2 == 2){
				char ni_username[20], new_password[20], buffer[200];
				item4:printf("Username : ");
				scanf("%s", &ni_username);
				if(ni_username){
					printf("New Password : ");
					scanf("%s", &new_password);
					if(strcmp(username, ni_username) == 0){
						sprintf(buffer, "UPDATE users SET password = '%s' WHERE username = '%s'", new_password, ni_username);
						int pass_change_query = mysql_query(con, buffer);
						if(pass_change_query == 0){
							printf("Password successfully changed.\n");
							goto item3;
						} else {
							fprintf(stderr, "%s\n", mysql_error(con));
						}
					} else {
						printf("Bad username type.");
					}
				}
			} else if(choice_2 == 3){
				char friend_name[30], buffer[200], buffer_2[200];
				printf("\n\n\t\t\tAdd friend :");
				scanf("%s", &friend_name);
				sprintf(buffer_2, "SELECT username FROM users WHERE username = '%s'", friend_name);
				int show_request_query = mysql_query(con, buffer_2);
				if(show_request_query == 0 ){
					MYSQL_RES *result = mysql_store_result(con);
					unsigned int num_fields = mysql_num_fields(result);
					my_ulonglong num_rows;
					num_rows = mysql_num_rows(result);
					if(num_rows > 0){
						char buffer_3[200];
						sprintf(buffer_3, "SELECT * FROM added_friend WHERE fromname = '%s' AND name = '%s'", username, friend_name);
						int compare_user_query = mysql_query(con, buffer_3);
						if(compare_user_query == 0){
							MYSQL_RES *result = mysql_store_result(con);
							unsigned int num_fields = mysql_num_fields(result);
							my_ulonglong num_rows;
							num_rows = mysql_num_rows(result);
							if(num_rows > 0){
								printf("Such a request includes.");
								goto item3;
							} else {
								sprintf(buffer, "INSERT INTO added_friend(fromname, name) VALUES('%s', '%s')", username, friend_name);
								int add_friend_query = mysql_query(con, buffer);
								if(add_friend_query == 0){
									printf("Friend added.\n");
									goto item3;
								} else {
									fprintf(stderr, "%s\n", mysql_error(con));
								}
							}
						} else {
							fprintf(stderr, "%s\n", mysql_error(con));
						}
					} else {
						printf("Wrong username.");
						goto item3;
					}
				} else {
					fprintf(stderr, "%s\n", mysql_error(con));
				}
			} else if(choice_2 == 4){
				char buffer[200];
				sprintf(buffer, "SELECT fromname FROM added_friend WHERE name = '%s'", username);
				int select_request_query = mysql_query(con, buffer);
				if(select_request_query == 0){
					MYSQL_RES *result = mysql_store_result(con);
					MYSQL_ROW row;
					unsigned int num_fields = mysql_num_fields(result);
					while((row = mysql_fetch_row(result))){
						for(int i = 0; i < num_fields;i++){
							printf("Friend request from : %s\n", row[i]);
						}
					}
					goto item3;
				} else {
					fprintf(stderr, "%s\n", mysql_error(con));
				}
			} else if(choice_2 == 5){
				char select_friend[20], buffer[200], buffer_2[200], buffer_3[300];
				printf("\n\n\t\t\tAccept friend with name :");
				scanf("%s", &select_friend);
				sprintf(buffer, "SELECT name FROM added_friend WHERE fromname = '%s' AND name = '%s'", select_friend, username);
				int select_friend_query = mysql_query(con, buffer);
				if(select_friend_query == 0){
					MYSQL_RES *result = mysql_store_result(con);
					unsigned int num_fields = mysql_num_fields(result);
					my_ulonglong num_rows;
					num_rows = mysql_num_rows(result);
					if(num_rows > 0){
						sprintf(buffer_3, "INSERT INTO infriend(friend, friend2) VALUES('%s', '%s')", select_friend, username);
						int insert_infriend_query = mysql_query(con, buffer_3);
						if(insert_infriend_query == 0){
							sprintf(buffer_2, "DELETE FROM added_friend WHERE fromname = '%s' AND name = '%s'", select_friend, username);
							int delete_from_friend_query = mysql_query(con, buffer_2);
							if(delete_from_friend_query == 0){
								printf("%s request was accepted.", select_friend);
								goto item3;
							} else {
								fprintf(stderr, "%s\n", mysql_error(con));
							}
						} else {
							fprintf(stderr, "%s\n", mysql_error(con));
						}
					} else {
						printf("Such a request do not includes.\n");
						goto item3;
					}
				} else {
					fprintf(stderr, "%s\n", mysql_error(con));
				}
			} else if(choice_2 == 6){
				char buffer[200];
				sprintf(buffer, "SELECT * FROM infriend WHERE friend = '%s' OR friend2 = '%s'", username, username);
				int select_request_query = mysql_query(con, buffer);
				MYSQL_RES *result = mysql_store_result(con);
				MYSQL_ROW row;
				unsigned int num_fields = mysql_num_fields(result);
				if(select_request_query == 0){
					while((row = mysql_fetch_row(result))){
						for(int i = 0; i < num_fields;i++){
							if(strcmp(row[i], username) == 0){
							} else {
								printf("Friend : %s\n", row[i]);
							}
						}
					}
					goto item3;
				} else {
					fprintf(stderr, "%s\n", mysql_error(con));
				}
			} else if(choice_2 == 7){
				char delete_choice[20], buffer[200], buffer_2[200];
				printf("\n\n\t\t\tDelete friend :");
				scanf("%s", &delete_choice);
				sprintf(buffer, "SELECT * FROM infriend WHERE friend = '%s' OR friend2 = '%s'", username, username);
				int select_request_query = mysql_query(con, buffer);
				MYSQL_RES *result = mysql_store_result(con);
				MYSQL_ROW row;
				unsigned int num_fields = mysql_num_fields(result);
				if(select_request_query == 0){
					while((row = mysql_fetch_row(result))){
						for(int i = 0; i < num_fields;i++){
							if(strcmp(row[i], username) == 0){
							} else {
								if(strcmp(row[i], delete_choice) == 0){
									sprintf(buffer_2, "DELETE FROM infriend WHERE friend = '%s' OR friend = '%s'", delete_choice, delete_choice);
									int delete_from_friend_query = mysql_query(con, buffer_2);
									if(delete_from_friend_query == 0){
										printf("%s was deleted.", delete_choice);
										goto item3;
									} else {
										fprintf(stderr, "%s\n", mysql_error(con));
									}
								}
							}
						}
					}
					goto item3;
				} else {
					fprintf(stderr, "%s\n", mysql_error(con));
				}

			}
		}
	} else {
		printf("\e[1;1H\e[2J");
		goto item;
	}

	mysql_close(con);
  return 0;

}
