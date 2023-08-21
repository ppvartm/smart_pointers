#include <iostream>
#include <memory>
#include <vector>
#include <algorithm>
#include "unique_ptr.h"
#include "shared_ptr.h"

int main()
{
	/*auto_ptr test*/
  std::auto_ptr<int> a_a(new int(3));
  std::cout << *a_a << "\n";

  std::auto_ptr<int> a_b = a_a;
  std::cout << *a_b << "\n";
 //  std::cout << *a_a << "\n";  //здесь ошибка, так как а отдал права пользовани€, теперь он null_pointer

 // std::auto_ptr<int> a_c =  std::auto_ptr<int>(a_a);  //здесь тa же ошибка, 
 // std::cout << *a_a << "\n";                          

  std::auto_ptr<int> a_d = std::auto_ptr<int>(new int(2)); //здесь временный объект, ошибки не будет, auto_ptr ничего не знает про rvalue

 // std::auto_ptr<int[]> a_e(new int(3));       // ошибка, так как auto_ptr не работает с массивами
  

     /*unique_ptr test*/
  std::unique_ptr<int> u_a(new int(5));
  std::cout << *u_a << "\n";

 // std::unique_ptr<int> u_b = u_aa; //ошибка, так как unique_ptr запрещает передачу прав пользовани€ пам€тью

  std::unique_ptr<int> u_c = std::unique_ptr<int>(new int(2)); //здесь ошибки не будет, так как unique_ptr понимает, что справа
  std::cout << *u_c << "\n";                                   // временный объект, который никак не может создавать опасность
   
  std::unique_ptr<int[]> a_e(new int(3));       // ошибки нет, так как unique_ptr умеет работать с массивами (в деструкторе будет
                                                // вызыван delete[])
  std::vector<std::unique_ptr<int>> u_vec(2);
  //u_vec.push_back(u_a);                         // ошибка компил€ции, так как произойдет присваивание unique_ptr
  u_vec.push_back(std::unique_ptr<int>(new int(2))); //ошибки нет, объект временный

     /*shared_ptr test*/
  std::shared_ptr<int> s_a(new int(7));
  std::shared_ptr<int> s_b(s_a);    // ошибки не будет, так как shared_ptr дл€ этого и создан (идет подсчет ссылок)
  std::cout << *s_a << "\n";
  std::cout << *s_b << "\n";
  
  std::vector<std::shared_ptr<int>> s_vec(10);
  s_vec.push_back(s_a);


  shared_ptr<int> sh_ptr = make_shared<int>(4);


}        

