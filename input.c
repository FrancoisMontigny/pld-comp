int main() {

  int i = 1;
  int a = 4;
  int j = 1, k=4, l=8, m=2;
  
  do {
    i = i + 2;  
  } while (a > i);


  int b = 42;

  i = test (i, i);

  return i;
}

/*int test(int a, int b) {
  return a*b;
}*/

int test(int a){
  if(a==1){
    return 1;
  } else {
    return a * test(a-1);
  }
  return 0;
}
