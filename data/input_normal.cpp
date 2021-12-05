/*
 * Created Date: 2021-10-12 22:02:30
 * Author: yuyangz
 */

//input test file with all symbol defined.

int main() 
{
    char s[] = "abc";
    double d = 1.2;
    int xVariable=1;
    int y = 2;
    int z = 0;
    if (xVariable <= 0){
        z = y-xVariable;
    } 
    else {
        z = xVariable++;
    }

    return 0;
}

//End of file.