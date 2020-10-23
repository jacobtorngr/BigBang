
void FileName(char * string,int number){
    int i=0;
    while(number){
        string[8-i]=48+(number%10);
        number/=10;
        i++;
    }
}
