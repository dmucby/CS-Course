int main(){
    register int grade = 80;
    register int level;
    if (grade >= 85){
        level = 1;
    } else if (grade >= 70){
        level = 2;
    } else if (grade >= 60){
        level = 3;
    } else{
        level = 4;
    }
    return level;
}