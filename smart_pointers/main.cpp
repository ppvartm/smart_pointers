#include <iostream>
#include <memory>



int main()
{
	/*auto_ptr test*/
  std::auto_ptr<int> a_a(new int(3));
  std::cout << *a_a << "\n";

  std::auto_ptr<int> a_b = a_a;
  std::cout << *a_b << "\n";
 //  std::cout << *a_a << "\n";  //здесь ошибка, так как а отдал права пользования, теперь он null_pointer

 // std::auto_ptr<int> a_c =  std::auto_ptr<int>(a_a);  //здесь тa же ошибка, 
 // std::cout << *a_a << "\n";                          

  std::auto_ptr<int> a_d = std::auto_ptr<int>(new int(2)); //здесь временный объект, ошибки не будет, auto_ptr ничего не знает про rvalue


     /*unique_ptr test*/
  std::unique_ptr<int> u_a(new int(5));
  std::cout << *u_a << "\n";

 // std::unique_ptr<int> u_b = u_aa; //ошибка, так как unique_ptr запрещает передачу прав пользования памятью

  std::unique_ptr<int> u_c = std::unique_ptr<int>(new int(2));
  std::cout << *u_c << "\n";
   

}