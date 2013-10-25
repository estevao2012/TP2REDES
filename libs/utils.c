void gotoxy(int x, int y)
{ 
	printf("\033[%d;%df",x,y);
}

void limpa_tela(){
	printf("\033[2J");
}