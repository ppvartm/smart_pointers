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
 //  std::cout << *a_a << "\n";  //����� ������, ��� ��� � ����� ����� �����������, ������ �� null_pointer

 // std::auto_ptr<int> a_c =  std::auto_ptr<int>(a_a);  //����� �a �� ������, 
 // std::cout << *a_a << "\n";                          

  std::auto_ptr<int> a_d = std::auto_ptr<int>(new int(2)); //����� ��������� ������, ������ �� �����, auto_ptr ������ �� ����� ��� rvalue

 // std::auto_ptr<int[]> a_e(new int(3));       // ������, ��� ��� auto_ptr �� �������� � ���������
  

     /*unique_ptr test*/
  std::unique_ptr<int> u_a(new int(5));
  std::cout << *u_a << "\n";

 // std::unique_ptr<int> u_b = u_aa; //������, ��� ��� unique_ptr ��������� �������� ���� ����������� �������

  std::unique_ptr<int> u_c = std::unique_ptr<int>(new int(2)); //����� ������ �� �����, ��� ��� unique_ptr ��������, ��� ������
  std::cout << *u_c << "\n";                                   // ��������� ������, ������� ����� �� ����� ��������� ���������
   
  std::unique_ptr<int[]> a_e(new int(3));       // ������ ���, ��� ��� unique_ptr ����� �������� � ��������� (� ����������� �����
                                                // ������� delete[])
  std::vector<std::unique_ptr<int>> u_vec(2);
  //u_vec.push_back(u_a);                         // ������ ����������, ��� ��� ���������� ������������ unique_ptr
  u_vec.push_back(std::unique_ptr<int>(new int(2))); //������ ���, ������ ���������

     /*shared_ptr test*/
  std::shared_ptr<int> s_a(new int(7));
  std::shared_ptr<int> s_b(s_a);    // ������ �� �����, ��� ��� shared_ptr ��� ����� � ������ (���� ������� ������)
  std::cout << *s_a << "\n";
  std::cout << *s_b << "\n";
  
  std::vector<std::shared_ptr<int>> s_vec(10);
  s_vec.push_back(s_a);


  shared_ptr<int> sh_ptr = make_shared<int>(4);


}        

