#ifndef _Customer_H
#define _Customer_H

class Customer
{
public:

    int GetAge() 
	{
		return m_age; 
	}
    
	void SetAge(int newAge)
	{
		m_age = newAge; 
	}
    
	std::string GetName() 
	{
		return m_name; 
	}
    
	void SetName(std::string newName) 
	{
		m_name = newName; 
	}

 private:

    int m_age;
    std::string m_name;
 
};

#endif