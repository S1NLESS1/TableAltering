#include <windows.h>
#include <stdio.h>
#include <conio.h>

struct my_struct{
	int number_of_factory;
	int number_of_branch;
	char surname_of_accountable[15];
	float availability_of_values;
	float amount_of_received;
	float amount_of_retired;
};

struct my_list{
	struct my_struct data;
	struct my_list *prev, *next;
};

struct my_struct create_data_main(); // Создание записи
int create_data(char *string); // вспом создание записи
struct my_list *create_entry(struct my_list *top, struct my_struct data); // Добавление записи
int check_list(struct my_list *top);			// Проверка пустоты файла

void print_list(struct my_list *top, int pos, int page, int mode); // Печать списка
void print_list_main(struct my_list *top); // вспом печать списка

struct my_list *delete_entry(struct my_list *top); // Удаление записи
void correct_entry(struct my_list *top);   // Корректировка записи
void sort(struct my_list *top, char *mass1[], char *mass2[]); // Сортировка списка

struct my_list *read_txt(struct my_list *top); // Чтение с текстового файла
void write_txt(struct my_list *top); // Запись в текстовый файл
struct my_list *read_bin(struct my_list *top); // Чтение с бинарного файла
void write_bin(struct my_list *top); // Запись в бинарный файл
int check_ext(int mode); // проверка на расширение файла и его открытие
void check_way(); // проверка наличия пути файла

void free_memory(struct my_list *top); // Освобождение памяти
void search(struct my_list *top, char *mass[4]); 	   //Поиск
void get_report(struct my_list *top, char *mass_report[3]); // Получение ведомостей

void print_menu(char *mass[12],int pos, int mode); // Подсвечивание меню(r = 1) и меню ведомостей(r = 2)
void print_entry(struct my_struct data,int pos_entry);
void print_line(char *string);						// Подсвечивание одной строки

int counter_klon = 0;
int count_of_entries = 0;
int count_of_pages = 0;
char file_name[260];
FILE *file = NULL;
								// C:\\Users\\Denchik\\Desktop\\File3.txt
int main(){
	printf("Hi. It is the program of accounting for the values of the plant\n");
	getch();
	
	struct my_struct entry;
	struct my_list *top = NULL;
    int pos = 1;
    int simbol = 0;
    
    char *mass[] = {
		"1) Create entry",
		"2) Print the list",
		"3) Delete entry",
		"4) Correct entry",
		"5) Search entry",
		"6) Sort the list",
		"7) Create table by txt file",
		"8) Save table in txt file",
		"9) Create table by binary file",
		"10) Save table in binary file",
		"11) Get report",
		"12) End the work program"
	};
	
	char *mass_report[] = {
		"    1) The value of valuables for each branch",
		"    2) The value of valuables throughout the plant",
		"    3) Exit"
	};
	
	char *mass_search[] = {
		"    1) Number of factory",
		"    2) Number of branch",
		"    3) Surname of accountable",
		"    4) Exit"
	};
	char *mass1_sort[] = {
		"    1) Number of factory",
		"    2) Number of branch",
		"    3) Surname of accountable",
		"    4) Amount of material assets",
		"    5) Amount of received",
		"    6) Amount of retired",
		"    7) Exit"
	};
	char *mass2_sort[] = {
		"    1) Ascending",
		"    2) Descending",
		"    3) Exit"
	};
	
    while(1){
    	system("cls");
    	if(simbol == 72) pos--;
    	else if(simbol == 80) pos++;
    	else if(simbol == 13) 
			switch(pos){
    			case 1: 
					entry = create_data_main();
					top = create_entry(top,entry);
					if(counter_klon) {
						printf("\n%d clone records have been found, the data for them has been updated",counter_klon);
						getch();
					}
					counter_klon = 0;
					break;
				case 2: print_list_main(top); break;
				case 3: top = delete_entry(top); break;
				case 4: correct_entry(top); break;
				case 5: search(top,mass_search); break;
				case 6: sort(top,mass1_sort,mass2_sort); break;
				case 7: top = read_txt(top); break;
				case 8: write_txt(top); break;
				case 9: top = read_bin(top); break;
				case 10: write_bin(top); break;
				case 11: get_report(top, mass_report); break;
				case 12: 
					free_memory(top);
					printf("Work with the program is completed. Goodbye!");
					return 0;
			}
		system("cls");
    	if(pos > 12) pos = 1;
    	else if(pos < 1) pos = 12;
    	print_menu(mass,pos,1);
    	simbol = getch();
	}
}

struct my_struct create_data_main(){
	struct my_struct data;
	char word[10];
		printf("Enter number of factory: ");
	data.number_of_factory = create_data("Number of factory");
		printf("Enter number of branch: ");
	data.number_of_branch = create_data("Number of branch");
		printf("Enter surname of accountable: ");
	scanf("%s",data.surname_of_accountable);
		printf("Enter the availability of values: ");
	scanf("%s",word);
	data.availability_of_values = atof(word);
		printf("Enter amount of received: ");
	scanf("%s",word);
	data.amount_of_received = atof(word);
		printf("Enter amount of retired: ");
	scanf("%s",word);
	data.amount_of_retired = atof(word);
	printf("The entry has been created\n");
	return data;
}

int create_data(char *string){
	char word[10];
	scanf("%s",word);
	int x = atoi(word);
	while(x > 1000 | x < 100){
		printf("%s must be more than 100 and less than 1000 and not contain letters\n Enter again: ",string);
		scanf("%s",word);
		x = atoi(word);
		system("cls");
	}
	return x;
}

struct my_list *create_entry(struct my_list *top, struct my_struct data){
	struct my_list *t;
	if(!top){
		top = (struct my_list*)malloc(sizeof(struct my_list));
		top->data = data;
		top->prev = NULL;
		top->next = NULL;
		count_of_entries++;
		count_of_pages = count_of_entries/11 + 1;
		return top;
	}
	
	for(t = top; t != NULL; t = t->next){
		if(t->data.number_of_factory == data.number_of_factory){
			if(t->data.number_of_branch == data.number_of_branch){
	 			t->data = data;
	 			counter_klon++;
				return top;
			}
		}
	}
	
	for(t = top; t != NULL; t = t->next){
		if(t->data.number_of_factory == data.number_of_factory){
			if(t->data.number_of_branch > data.number_of_branch){
				if(t->prev != NULL){
					t = t->prev;
					t->next->prev = (struct my_list*)malloc(sizeof(struct my_list));
					t->next->prev->data = data;
					t->next->prev->next = t->next;
					t->next->prev->prev = t;
					t->next = t->next->prev;
					count_of_entries++;
					count_of_pages = count_of_entries/11 +1;
					return top;
				}
				t->prev = (struct my_list*)malloc(sizeof(struct my_list));
				t->prev->data = data;
				t->prev->next = t;
				t->prev->prev = NULL;
				count_of_entries++;
				count_of_pages = count_of_entries/11 +1;
				return t->prev;
			}
			else if(t->next != NULL){
				if( t->next->data.number_of_factory != data.number_of_factory){
					t->next->prev = (struct my_list*)malloc(sizeof(struct my_list));
					t->next->prev->data = data;
					t->next->prev->next = t->next;
					t->next->prev->prev = t;
					t->next = t->next->prev;
					count_of_entries++;
					count_of_pages = count_of_entries/11 +1;
					return top;
				}
			}
		}
	}
	for(t = top; t->next != NULL; t = t->next);
	t->next = (struct my_list*)malloc(sizeof(struct my_list));
	t->next->data = data;
	t->next->prev = t;
	t->next->next = NULL;
	count_of_entries++;
	count_of_pages = count_of_entries/11 +1;
	return top;
}

int check_list(struct my_list *top){
	if (!top){
		printf("List is empty\n");
		getch();
		return 1;
	}
	else return 0;
}

void print_list_main(struct my_list *top){
	
	if(check_list(top)) return;
	
	int i = 1;
	int simbol = 0;
	int page = 1;
	while(1){
		system("cls");
		if(simbol == 75) page--;
    	else if(simbol == 77) page++;
    	if(simbol == 13) return;
    	
    	if (page > count_of_pages) page = count_of_pages;
    	else if(page < 1) page = 1;
    	
    	print_list(top,0,page,0);
    	simbol = getch();
	}
}

void print_list(struct my_list *top, int pos, int page, int mode){
	HANDLE hStdOut = GetStdHandle(STD_OUTPUT_HANDLE);
	
	if(check_list(top)) return;
	
	struct my_list *t = top;
	int i = 1;
	printf("|  N  |  N factory  | N branch | Surname of accountable | Availability of values | Amount of received | Amount of retired |\n");
	for(int p = 1; p < page; p++) for(int ent = 1; ent < 11; ent++, i++,t = t->next);
	for(int ent = 1; ent < 11 & t != NULL; ent++, t = t->next, i++){
		if(mode == 1 & ent == pos){
			SetConsoleTextAttribute(hStdOut,BACKGROUND_GREEN|BACKGROUND_RED|BACKGROUND_BLUE);
			printf("| %-3.d | %-11.d | %-8.d | %-22s | %-22.3f | %-18.3f | %-17.3f |",i,t->data.number_of_factory,t->data.number_of_branch,t->data.surname_of_accountable,t->data.availability_of_values,t->data.amount_of_received,t->data.amount_of_retired);
			SetConsoleTextAttribute(hStdOut,FOREGROUND_GREEN|FOREGROUND_RED|FOREGROUND_BLUE);
			printf("\n");
			continue;
		}
		printf("| %-3.d | %-11.d | %-8.d | %-22s | %-22.3f | %-18.3f | %-17.3f |\n",i,t->data.number_of_factory,t->data.number_of_branch,t->data.surname_of_accountable,t->data.availability_of_values,t->data.amount_of_received,t->data.amount_of_retired);
	}
	printf("                                                         << %d >>                                                          \n",page);
	
	if(!mode){
		print_line("Exit");
	}
	
	return;
}

void print_line(char *string){
	HANDLE hStdOut = GetStdHandle(STD_OUTPUT_HANDLE);
	
	SetConsoleTextAttribute(hStdOut,BACKGROUND_GREEN|BACKGROUND_RED|BACKGROUND_BLUE);
    printf("%s ",string);
    SetConsoleTextAttribute(hStdOut,FOREGROUND_GREEN|FOREGROUND_RED|FOREGROUND_BLUE);
}

struct my_list *delete_entry(struct my_list *top){
	struct my_list *t;
	int pos = 1, page = 1;
	int simbol = 0;
	int choose_del = 1;
	int p;
	
	if(check_list(top)) return top;
	
	while(1){
		mark:
		system("cls");
		printf("Choose which entry will be delete: \n");
		
    	if(simbol == 72) pos--;
    	else if(simbol == 80) pos++;
		else if(simbol == 75) page--;
    	else if(simbol == 77) page++;
    	else if(simbol == 13) {
    		choose_del = 1;
    		simbol = 0;
    		
    		if(page < count_of_pages){
    			if(pos == 11) return top;
			}
			else{
				if(pos == count_of_entries%10 + 1) return top;
			}
			
			while(1){
				if(simbol == 75) choose_del--;
    			else if(simbol == 77) choose_del++;
    			else if(simbol == 13) {
    				
    				if(choose_del == 2){
    					simbol = 0;
    					goto mark;
					}
					
    				if(pos == 1 & page == 1){
						if(top->next == NULL){
							free(top);
							return NULL;
						}
						top = top->next;
						free(top->prev);
						goto end;
					}
					
					for(p = 1,t = top; p < page; p++) for(int ent = 1; ent < 11 & t != NULL; ent++, t = t->next);
    				
					for(int i = 1; i < pos; i++, t = t->next);
    				
    				if(t->next == NULL){
						t->prev->next = NULL;
						free(t);
						count_of_entries--;
						count_of_pages = count_of_entries/11 +1;
						top->prev = NULL;
						system("cls");
						printf("Entry has been deleted\n");
						getch();
						return top;
					}
					
    				t->prev->next = t->next;
    				t->next->prev = t->prev;
    				free(t);
    				goto end;
    			}
    			
				if(choose_del < 1) choose_del = 1;
				else if(choose_del > 2) choose_del = 2;
				system("cls");
				printf("Are you sure you want to delete this entry?\n");
				
				if(choose_del == 1){
					print_line("|   YES   |");
					printf("   NO   |");
				}
				else{
    				printf("|   YES   ");
    				print_line("|   NO   |");
				}
				
				simbol = getch();
			}
		}
		if (page > count_of_pages) page = count_of_pages;
    	else if(page < 1) page = 1;
    	
		if(page < count_of_pages){
			if(pos > 11) pos = 1;
			else if(pos < 1) pos = 11;
		}
		else {
			if(pos > count_of_entries%10 + 1) pos = 1;
			else if(pos < 1) pos = count_of_entries%10 + 1;
		}
		
    	print_list(top,pos,page,1);
    	
    	if((page < count_of_pages  & pos == 11) | (page == count_of_pages & pos == count_of_entries%10 + 1)){
			print_line("Exit");
    	}
    	else printf("Exit");
    	
    	simbol = getch();
	}
	end:
		count_of_entries--;
		count_of_pages = count_of_entries/11 +1;
		top->prev = NULL;
		system("cls");
		printf("Entry has been deleted\n");
		getch();
		return top;
}

void correct_entry(struct my_list *top){
	struct my_list *t, *j;
	struct my_struct data;
	char corrected[10];
	int pos = 1, page = 1, pos_entry = 1;
	int simbol = 0, simbol_entry = 0;
	int flag = 0;
	int counter = 1, counter2 = 1;
	
	if(check_list(top)) return;

	while(1){
		mark:
		system("cls");
		printf("Choose which entry will be correct: \n");
    	if(simbol == 72) pos--;
    	else if(simbol == 80) pos++;
		else if(simbol == 75) page--;
    	else if(simbol == 77) page++;
    	else if(simbol == 13) {
    		if(page < count_of_pages){
    			if(pos == 11) return;
			}
			else{
				if(pos == count_of_entries%10 + 1) return;
			}
			
    		t = top;
    		pos_entry = 1;
    		simbol_entry = 0;
    		counter = 1;
    		for(int p = 1; p < page; p++) for(int ent = 1; ent < 11; ent++, t = t->next, counter++);
    		for(int j = 1; j < pos; t = t->next, j++, counter++);
    		while(1){
    			system("cls");
    			counter2 = 1;
    			if(simbol_entry == 72) pos_entry--;
    			else if(simbol_entry == 80) pos_entry++;
    			else if(simbol_entry == 13){
    				switch(pos_entry){
    					case 1: 
    						data = t->data;
							printf("Enter the new number of factory: ");
							data.number_of_factory = create_data("Number of factory");
							system("cls");
							
							for(j = top; j != NULL; j = j->next, counter2++){
								if((j->data.number_of_factory == data.number_of_factory) & (j->data.number_of_branch == data.number_of_branch) & (counter != counter2) ){
									printf("Error: A record with this factory and branch number already exists. Exit...\n");
									getch();
									return;
								}
							}
							
							t->data.number_of_factory = data.number_of_factory;
							system("cls");
							break;
							
    					case 2:
    						data = t->data;
							printf("Enter the new number of branch: ");
							data.number_of_branch = create_data("Number of branch");
							
							for(j = top; j != NULL; j = j->next,counter2++){
								if( (j->data.number_of_factory == data.number_of_factory) & (j->data.number_of_branch == data.number_of_branch) & (counter != counter2) ){
									printf("Error: A record with this factory and branch number already exists. Exit...\n");
									getch();
									return;
								}
							}
							
							system("cls");
							t->data.number_of_branch = data.number_of_branch;
							break;
							
    					case 3: 							
							printf("Enter the new surname of accountable: ");
							scanf("%s",&t->data.surname_of_accountable);
							system("cls");
							break;
							
    					case 4: 							
							printf("Enter the new availability of values: ");
							scanf("%s",corrected);
							t->data.availability_of_values = atof(corrected);
							system("cls");
							break;
							
    					case 5: 							
							printf("Enter the new amount of received: ");
							scanf("%s",corrected);
							t->data.amount_of_received = atof(corrected);
							system("cls");
							break;
							
    					case 6: 							
							printf("Enter the new amount of retired: ");
							scanf("%s",corrected);
							t->data.amount_of_retired = atof(corrected);
							system("cls");
							break;
							
    					case 7: 
    						simbol = 0;
							goto mark;
							break;
					}
				}
    			if(pos_entry > 7) pos_entry = 1;
    			else if(pos_entry < 1) pos_entry = 7;
    			printf("Which field do you want to correct?\n");
    			print_entry(t->data,pos_entry);
    			simbol_entry = getch();
    		}
		}
		if (page > count_of_pages) page = count_of_pages;
    	else if(page < 1) page = 1;
    	
		if(page < count_of_pages){
			if(pos > 11) pos = 1;
			if(pos < 1) pos = 11;
		}
		else {
			if(pos > count_of_entries%10 + 1)pos = 1;
			if(pos < 1) pos = count_of_entries%10 + 1;
		}
		
    	print_list(top,pos,page,1);
    	
    	if((page < count_of_pages & pos == 11) | (page == count_of_pages & pos == count_of_entries%10 + 1)){
			print_line("Exit");
    	}
    	else printf("Exit");
    	
    	simbol = getch();
	}
	
}

void sort(struct my_list *top, char *mass1[7], char *mass2[]){
	
	if(check_list(top)) return ;
	
	struct my_list *t, *j;
	struct my_struct temp;
	int flg;
	int simbol = 0, simbol_mode = 0;
	int pos = 1, pos_mode = 1;
	int massage = 0;
	while(1){
		mark:
		system("cls");
		printf("Choose which way the list will be sorted: \n");
    	if(simbol == 72) pos--;
    	else if(simbol == 80) pos++;
    	else if(simbol == 13) {
    		
    		if(pos == 3) {
    			system("cls");
				return ;
			}
			
    		pos_mode = 1;
    		simbol_mode = 0;
    		while(1){
    			system("cls");
				printf("Choose what kind of sort you want to do: \n");
    			if(simbol_mode == 72) pos_mode--;
    			else if(simbol_mode == 80) pos_mode++;
    			else if(simbol_mode == 13){
    				if(!top->next){
    					printf("The list has only one entry\n");
    					return ;
					}
					for (t = top; t != NULL; t = t->next) {
						for (j = top; j->next != NULL; j = j->next) {
							flg = 0;
    						switch(pos_mode){
    							case 1: 
									if(pos == 1) {	
										if(j->data.number_of_factory > j->next->data.number_of_factory) flg = 1;
									}
    								else if(j->data.number_of_factory < j->next->data.number_of_factory) flg = 1; 
									break;
								case 2: 
									if(pos == 1) {
										if(j->data.number_of_branch > j->next->data.number_of_branch) flg = 1; 
									}
									else if(j->data.number_of_branch < j->next->data.number_of_branch) flg = 1;
									break;
								case 3: 
									if(pos == 1) {
										if(strcmp(j->data.surname_of_accountable,j->next->data.surname_of_accountable) > 0) flg = 1;
									}
									else  if(strcmp(j->data.surname_of_accountable,j->next->data.surname_of_accountable) < 0) flg = 1;
									break;
								case 4: 
								    if(pos == 1) {
										if(j->data.availability_of_values > j->next->data.availability_of_values) flg = 1;
									}
								    else if(j->data.availability_of_values < j->next->data.availability_of_values) flg = 1;
									break;
								case 5: 
									if(pos == 1) {
										if(j->data.amount_of_received > j->next->data.amount_of_received) flg = 1;
									}
									else if(j->data.amount_of_received < j->next->data.amount_of_received) flg = 1;
									break;
								case 6: 
									if(pos == 1) {
										if(j->data.amount_of_retired > j->next->data.amount_of_retired) flg = 1;
									}
									else if(j->data.amount_of_retired < j->next->data.amount_of_retired) flg = 1;
									break;
    							case 7: 
    								simbol = 0;
									goto mark;
									break;
							}
							if (flg) {
								temp = j->data;
								j->data = j->next->data;
								j->next->data = temp;
								massage = 1;
							}
						}
					} 
					  
					system("cls");
					
					if(massage)	printf("The list was sorted\n");
					else printf("List is already sorted\n");
					getch();
					return ;		
    			}
    			if(pos_mode > 7) pos_mode = 1;
   				else if(pos_mode < 1) pos_mode = 7;
   				print_menu(mass1,pos_mode,4);
   				simbol_mode = getch();
			}
		}
		if(pos > 3) pos = 1;
    	else if(pos < 1) pos = 3;
    	print_menu(mass2,pos,2);
    	simbol = getch();
	}
}

struct my_list *read_txt(struct my_list *top){
 
	struct my_struct entry;
	int n = 0, flg = 0, count = 0, flg_read = 0;
	char word1[4],word2[4],word3[10],word4[10],word5[10];
	int skobka;
	int pos = 1, simbol = 0;
	
	if(top) {
		while(1){
			system("cls");
			printf("The table already exists. Delete it or add to it?\n");
			if(simbol == 75) pos--;
    		else if(simbol == 77) pos++;
    		else if(simbol == 13) {
    			if(pos == 1){
    				free_memory(top);
    				top = NULL;
    				break;
				}
				else if(pos == 2){
					break;
				}
    			else {
    				system("cls");
					return top;
				}
			}
			
			if(pos > 3) pos = 3;
    		else if(pos < 1) pos = 1;
    		
    		if(pos == 1){
					print_line("|   Delete   |");
					printf("   Add   |   Exit   |");
				}
				else if(pos == 2){
    				printf("|   Delete   ");
    				print_line("|   Add   |");
    				printf("   Exit   |");
				}
				else{
					printf("|   Delete   |   Add   ");
					print_line("|   Exit   |");
				}
				simbol = getch();
				system("cls");
			}
	}
	
	printf("Enter the name of file which will be used to create the table: ");
	scanf("%s",file_name);
	if(check_ext(1)) return top;
	
	while(1){
		flg = 0;
		n = fscanf(file,"%s %s %s %s %s %s %s", &skobka,word1, word2, entry.surname_of_accountable, word3, word4, word5);
		if (n == EOF) break;
		flg_read = 1;
		entry.number_of_factory = atoi(word1);
		entry.number_of_branch = atoi(word2);
		
		if(entry.number_of_factory > 1000 | entry.number_of_factory < 100) flg = 1;
		else if(entry.number_of_branch > 1000 | entry.number_of_branch < 100) flg = 1;
		if(flg) {
			count++;
			continue;
		}
		
		entry.availability_of_values = atof(word3);
		entry.amount_of_received = atof(word4);
		entry.amount_of_retired = atof(word5);
		top = create_entry(top,entry);
	}
	
	if(flg_read){
		printf("Table has been created by the text file\n");
		if(count) printf("%d entries were not add to the list because they have incorrect data",count);
		if(counter_klon) printf("\n%d clone records have been found, the data for them has been updated",counter_klon);
	}
	else printf("The file is empty\n");
	
	counter_klon = 0;
	getch();
	fclose(file);
	return top;
}

void write_txt(struct my_list *top){
	struct my_list *t;
	int i = 1;
	
	if(check_list(top)) return;
	
	printf("Enter the name of file which will be used to write the table: ");
	scanf("%s",file_name);
	if(check_ext(2)) return;
	check_way();
	
	for(t=top; t != NULL; t = t->next,i++){
		fprintf(file, "%d) %d %d %s %.3f %.3f %.3f\n",i,t->data.number_of_factory,t->data.number_of_branch,t->data.surname_of_accountable,t->data.availability_of_values,t->data.amount_of_received,t->data.amount_of_retired);
	}
	
	printf("File has been written in the text file\n");
	//if(!(strlen(file_name)-4)) printf(" and placed in the app folder");
	getch();
	fclose(file);
	return;
}

struct my_list *read_bin(struct my_list *top){
	struct my_struct entry;
	int n = 0,flg_read = 0, count = 0;
	int simbol = 0, simbol_mode = 0;
	int pos = 1, pos_mode = 1;
	
	if(top) {
		while(1){
			system("cls");
			printf("The table already exists. Delete it or add to it?\n");
			if(simbol == 75) pos--;
    		else if(simbol == 77) pos++;
    		else if(simbol == 13) {
    			if(pos == 1){
    				free_memory(top);
    				top = NULL;
    				break;
				}
				else if(pos == 2){
					break;
				}
    			else {
    				system("cls");
					return top;
				}
			}
			
			if(pos > 3) pos = 3;
    		else if(pos < 1) pos = 1;
    		
    		if(pos == 1){
					print_line("|   Delete   |");
					printf("   Add   |   Exit   |");
				}
				else if(pos == 2){
    				printf("|   Delete   ");
    				print_line("|   Add   |");
    				printf("   Exit   |");
				}
				else{
					printf("|   Delete   |   Add   ");
					print_line("|   Exit   |");
				}
				
				simbol = getch();
				system("cls");
			}
	}
	
	printf("Enter the name of file which will be used to create the table: ");
	scanf("%s",file_name);
	if (check_ext(3))return top;
	
	while(1){
		n = fread(&entry,sizeof(struct my_struct),1,file);
		if (n < 1) break;
		top = create_entry(top,entry);
		flg_read = 1;
	}
	
	if(flg_read) printf("The table was created by binary file\n");
	else printf("The file is empty\n"); 
	
	getch();
	fclose(file);
	return top;
}

void write_bin(struct my_list *top){
	struct my_list *t;
	
	if(check_list(top)) return;
	
	printf("Enter the name of file which will be used to write the table: ");
	scanf("%s",file_name);
	if (check_ext(4)) return;
	check_way();
	
	for(t=top; t != NULL; t = t->next){
		fwrite(&t->data,sizeof(struct my_struct),1,file);
	}
	
	printf("File has been written in the binary file\n");
	getch();
	fclose(file);
	return;
}

int check_ext(int mode){
	char check[4];
	int letters = strlen(file_name);
	char txt[] = ".txt";
	char bin[] = ".bin";
	int flg_check = 0;
	
	for(int i = 4; i > 0 ; i--) check[4-i] = file_name[letters - i];
	
	if(mode <= 2) {
		for(int i = 0; i < 4; i++){
			if(txt[i] != check[i]) flg_check = 1;
		}
	}
	else {
		for(int i = 0; i < 4; i++){
			if(bin[i] != check[i]) flg_check = 1;
		}
	}
	
	if(flg_check){
		system("cls");
		if(mode <= 2) printf("Error:  The file extension does not match \"%.4s\"\n",txt);
		else printf("Error:  The file extension does not match \"%.4s\"\n",bin);
		getch();
		return 1;
	}
	
	fclose(file);
	switch(mode){
		case 1: file = fopen(file_name,"rt"); break;
		case 2: file = fopen(file_name,"wt"); break;
		case 3: file = fopen(file_name,"rb"); break;
		case 4: file = fopen(file_name,"wb"); break;
	}
	
	if(!file){
		printf("Error: File opening error\nThe system probably can't find the file.\n");
		getch();
		return 1;
	}
	
	return 0;
}

void check_way(){
	int flg = 0;
	
	switch(file_name[0]){
		case 'C': case 'D': case 'E': case 'F': case 'G': break;
		default: flg = 1; break;
	}
	
	if(file_name[1] == ':') flg = 0;
	else flg = 1;
	
	if(flg){
		system("cls");
		printf("Warning: the file path was not specified - the file will be created in the directory with the program.\n");
		getch();
		return;
	}
	
	return;
}

void free_memory(struct my_list *top){
	struct my_list *t = top;
	
	if(!top){
		return;
	}

	while (t != NULL){
		top = t;
		t = t->next;
		free(top);
		count_of_entries--;
		count_of_pages = count_of_entries/11 + 1;
	}
	return;
}

void search(struct my_list *top, char *mass[4]){
	struct my_list *t;
	
	if(check_list(top)) return;
	
	int pos = 1, page = 1, pages;
	int simbol = 0;
	int i = 1, p_count = 0;
	int choose;
	char choose_n[15];
	int p = 1, found = 0, flg = 0;
	
	while(1){
		system("cls");
		mark:
		printf("Which entry do you want to search? Choose the search field: \n");
    	if(simbol == 72) pos--;
    	else if(simbol == 80) pos++;
    	else if(simbol == 13) {
    		system("cls");
    		found = 0;
    		switch(pos){
    			case 1: 
					printf("Enter the number of factory: ");
					scanf("%d",&choose);
					break;
    			case 2:
					printf("Enter the number of branch: ");
					scanf("%d",&choose);
					break;
    			case 3: 
					printf("Enter the surname of accountable: ");
					scanf("%s",choose_n);
					break;
    			case 4: return;
    		}
    		
    		for(t = top; t != NULL; t = t->next) {
				if(pos == 1) {
					if(t->data.number_of_factory == choose) found++;
				}
				else if(pos == 2){
					if(t->data.number_of_branch == choose) found++;
				}
				else {
					if(strcmp(t->data.surname_of_accountable, choose_n) == 0) found++;
				}
			}
    		
    		if(!found){
				system("cls");
				printf("Entries were not found\n");
				simbol = 0;
				getch();
				goto mark;
			}
			
			pages = found/11 + 1;
			simbol = 0;
			
			while(1){
				system("cls");
				i = 1;
				
				if(simbol == 75) page--;
				else if(simbol == 77) page++;
				else if(simbol == 13){
					simbol = 0;
					system("cls");
					goto mark;
				}
				
				if(page < 1) page = 1;
				else if(page > pages) page = pages;
				
				for(t = top, p = 1; p < page; t = t->next,i++){
					if (pos == 1){
						if(t->data.number_of_factory == choose) p_count++;	
					}
					else if(pos == 2){
						if(t->data.number_of_branch == choose) p_count++;
					}	
					else {
						if(strcmp(t->data.surname_of_accountable, choose_n) == 0) p_count++;
					}
					if (p_count == 10) {
						p++;
						p_count = 0;
					}
				}
				
				printf("|  N  |  N factory  | N branch | Surname of accountable | Availability of values | Amount of received | Amount of retired |\n");
				for(int ent = 0; ent < 10 & t != NULL; t = t->next,i++){
					flg = 0;
					
					if (pos == 1){
						if(t->data.number_of_factory == choose) flg = 1;	
					}
					else if(pos == 2){
						if(t->data.number_of_branch == choose) flg = 1;
					}	
					else {
						if(strcmp(t->data.surname_of_accountable, choose_n) == 0) flg = 1;
					}
					
					if(flg){
						printf("| %-3.d | %-11.d | %-8.d | %-22s | %-22.3f | %-18.3f | %-17.3f |\n",i,t->data.number_of_factory,t->data.number_of_branch,t->data.surname_of_accountable,t->data.availability_of_values,t->data.amount_of_received,t->data.amount_of_retired);
						ent++;
					}
				}
				printf("                                                         << %d >>                                                          \n",page);
				
				print_line("Exit");
    			simbol = getch();
			}
		}
		if(pos > 4) pos = 1;
    	else if(pos < 1) pos = 4;
    	print_menu(mass,pos,3);
    	simbol = getch();
    }
}

void get_report(struct my_list *top, char *mass_report[3]){
	
	if(check_list(top)) return ;
	
	struct my_list *t;
	struct my_struct data;
	data.amount_of_received = 0;
	data.amount_of_retired = 0;
	data.availability_of_values = 0;
	int simbol = 0, factory, found = 0;
	int pos = 1, page = 1, pages, p; // f - флаг для определения наличия данного завода/филиала
	
	while(1){
		system("cls");
		mark:
    	if(simbol == 72) pos--;
   		else if(simbol == 80) pos++;
   		else if(simbol == 13) {
   			if(pos != 3){
   				printf("Enter the name of the file that will be used to output the statements: ");
				scanf("%s",file_name);
				int check = check_ext(2);
				//check_way();
				if(check) return;
			}
			
			found = 0;
			int i = 1;
			switch(pos){
   				case 1:
					printf("Enter the number of factory: ");
					scanf("%d",&factory);
					fprintf(file,"|  N  | N branch | Availability of values | Amount of received | Amount of retired |\n");
					for(t = top; t != NULL; t = t->next){
						if (t->data.number_of_factory == factory) {
							fprintf(file, "| %-3.d | %-8.d | %-22.3f | %-18.3f | %-17.3f |\n",i,t->data.number_of_branch,t->data.availability_of_values,t->data.amount_of_received,t->data.amount_of_retired);
							i++;
							found++;
						}
					}
					
					if(!found){
						system("cls");
						printf("The factory was not found\n");
						getch();
						simbol = 0;
						goto mark;
					}
					pages = found/11 + 1;
					simbol = 0;
					while(1){
						i = 1;
						system("cls");
						if(simbol == 75) page--;
						else if(simbol == 77) page++;
						else if(simbol == 13){
							simbol = 0;
							system("cls");
							goto mark;
						}
						if(page < 1) page = 1;
						else if(page > pages) page = pages;
						int p_ent = 0;
						for(t = top, p = 1; p < page; t = t->next) {	
							if(t->data.number_of_factory == factory) p_ent++;	
							if (p_ent == 10) {
								p++;
								p_ent = 0;
							}
						}
						printf("|  N  | N branch | Availability of values | Amount of received | Amount of retired |\n");
						for(int ent = 0; ent < 10 & t != NULL; t = t->next){	
							if(t->data.number_of_factory == factory){
								printf("| %-3.d | %-8.d | %-22.3f | %-18.3f | %-17.3f |\n",i,t->data.number_of_branch,t->data.availability_of_values,t->data.amount_of_received,t->data.amount_of_retired);
								ent++;
								i++;
							}
						}
						printf("                                  << %d >>                                  \n",page);
						print_line("Exit");
    					simbol = getch();
					}
						break;
					case 2: 
						printf("Enter the number of factory: ");
						scanf("%d",&factory);
						for(t = top; t != NULL; t = t->next){
							if (t->data.number_of_factory == factory) {
								found++;
								break;
							}
						}
						if(!found){
							system("cls");
							printf("The factory was not found\n");
							getch();
							simbol = 0;
							goto mark;
							return;
						}
						for(t = top; t != NULL;t = t->next)
							if(t->data.number_of_factory == factory){
								data.availability_of_values += t->data.availability_of_values;
								data.amount_of_received += t->data.amount_of_received;
								data.amount_of_retired += t->data.amount_of_retired;
							}
							system("cls");
							printf("___________________________________________________________________\n");
							printf("|                          Factory N %d                          |\n| Availability of values | Amount of received | Amount of retired |\n",factory);
							printf("| %-22.3f | %-18.3f | %-17.3f |\n",data.availability_of_values,data.amount_of_received,data.amount_of_retired);
							printf("___________________________________________________________________\n");
							
							fprintf(file,"___________________________________________________________________\n");
							fprintf(file,"|                          Factory N %d                          |\n| Availability of values | Amount of received | Amount of retired |\n",factory);
							fprintf(file,"| %-22.3f | %-18.3f | %-17.3f |\n",data.availability_of_values,data.amount_of_received,data.amount_of_retired);
							fprintf(file,"___________________________________________________________________\n");
							print_line("Exit");
							getch();
							system("cls");
						break;
					case 3: 
						system("cls");
						return;
			}
		}
   	if(pos > 3) pos = 1;
   	else if(pos < 1) pos = 3;
   	printf("Choose what kind of report you want to get: \n");
   	print_menu(mass_report,pos,2);
   	simbol = getch();
   }
}

void print_menu(char *mass[],int pos, int mode){
	int j;
	
	switch(mode){
		case 1: j = 12; break;
		case 2: j = 3; break;
		case 3: j = 4; break;
		case 4: j = 7; break;
	}

	pos--;
	
	for(int i = 0; i < j; i++){
		if(i == pos) {
			print_line(mass[i]);
			printf("\n");
			continue;
		}
		printf("%s\n", mass[i]);
	}
	
	return;
}

void print_entry(struct my_struct data,int pos_entry){
	HANDLE hStdOut = GetStdHandle(STD_OUTPUT_HANDLE);
	
	switch(pos_entry){
		case 1: 
			SetConsoleTextAttribute(hStdOut,BACKGROUND_GREEN|BACKGROUND_RED|BACKGROUND_BLUE);
			printf("Number of factory: %d", data.number_of_factory);
			SetConsoleTextAttribute(hStdOut,FOREGROUND_GREEN|FOREGROUND_RED|FOREGROUND_BLUE);
			printf("\nNumber of branch: %d\nSurname of accountable: %s\nAvailability of values: %.3f\nAmount of received: %.3f\nAmount of retired: %.3f\nExit",data.number_of_branch,data.surname_of_accountable,data.availability_of_values,data.amount_of_received,data.amount_of_retired);
			break;
		case 2: 
			printf("Number of factory: %d\n", data.number_of_factory);
			SetConsoleTextAttribute(hStdOut,BACKGROUND_GREEN|BACKGROUND_RED|BACKGROUND_BLUE);
			printf("Number of branch: %d", data.number_of_branch);
			SetConsoleTextAttribute(hStdOut,FOREGROUND_GREEN|FOREGROUND_RED|FOREGROUND_BLUE);
			printf("\nSurname of accountable: %s\nAvailability of values: %.3f\nAmount of received: %.3f\nAmount of retired: %.3f\nExit",data.surname_of_accountable,data.availability_of_values,data.amount_of_received,data.amount_of_retired);
			break;
		case 3: 
			printf("Number of factory: %d\nNumber of branch: %d\n", data.number_of_factory,data.number_of_branch);
			SetConsoleTextAttribute(hStdOut,BACKGROUND_GREEN|BACKGROUND_RED|BACKGROUND_BLUE);
			printf("Surname of accountable: %s", data.surname_of_accountable);
			SetConsoleTextAttribute(hStdOut,FOREGROUND_GREEN|FOREGROUND_RED|FOREGROUND_BLUE);
			printf("\nAvailability of values: %.3f\nAmount of received: %.3f\nAmount of retired: %.3f\nExit",data.availability_of_values,data.amount_of_received,data.amount_of_retired);
			break;
		case 4: 
			printf("Number of factory: %d\nNumber of branch: %d\nSurname of accountable: %s\n", data.number_of_factory,data.number_of_branch,data.surname_of_accountable);
			SetConsoleTextAttribute(hStdOut,BACKGROUND_GREEN|BACKGROUND_RED|BACKGROUND_BLUE);
			printf("Availability of values: %.3f", data.availability_of_values);
			SetConsoleTextAttribute(hStdOut,FOREGROUND_GREEN|FOREGROUND_RED|FOREGROUND_BLUE);
			printf("\nAmount of received: %.3f\nAmount of retired: %.3f\nExit",data.amount_of_received,data.amount_of_retired);
			break;
		case 5: 
			printf("Number of factory: %d\nNumber of branch: %d\nSurname of accountable: %s\nAvailability of values: %.3f\n", data.number_of_factory,data.number_of_branch,data.surname_of_accountable,data.availability_of_values);
			SetConsoleTextAttribute(hStdOut,BACKGROUND_GREEN|BACKGROUND_RED|BACKGROUND_BLUE);
			printf("Amount of received: %.3f", data.amount_of_received);
			SetConsoleTextAttribute(hStdOut,FOREGROUND_GREEN|FOREGROUND_RED|FOREGROUND_BLUE);
			printf("\nAmount of retired: %.3f\nExit",data.amount_of_retired);
			break;
		case 6:
			printf("Number of factory: %d\nNumber of branch: %d\nSurname of accountable: %s\nAvailability of values: %.3f\nAmount of received: %.3f\n", data.number_of_factory,data.number_of_branch,data.surname_of_accountable,data.availability_of_values,data.amount_of_received);
			SetConsoleTextAttribute(hStdOut,BACKGROUND_GREEN|BACKGROUND_RED|BACKGROUND_BLUE);
			printf("Amount of retired: %.3f", data.amount_of_retired);
			SetConsoleTextAttribute(hStdOut,FOREGROUND_GREEN|FOREGROUND_RED|FOREGROUND_BLUE);
			printf("\nExit");
			break;
		case 7:
			printf("Number of factory: %d\nNumber of branch: %d\nSurname of accountable: %s\nAvailability of values: %.3f\nAmount of received: %.3f\nAmount of retired: %.3f\n", data.number_of_factory,data.number_of_branch,data.surname_of_accountable,data.availability_of_values,data.amount_of_received,data.amount_of_retired);
			print_line("Exit");
			break;
	}	
	return;
}
