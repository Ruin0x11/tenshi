void main()
{
  print("Hello world\n");
  
  int a = 0;
  bool success = tenshi_getvar("b", a);
  hsp_dialog(success + ": 'a' is " + a);
}