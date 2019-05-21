#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include<list>
#define MAX 30
using std::list;

class UStack
{
    
    private:
    protected:
    char unitName[MAX];
    
    int attack;
    int defense;
    int mindamage;
    int maxdamage;
    int speed;
    int health;
    bool shooter;
    bool flyer;
    
    int hexes15, hexes11; // x, y-coords in hexagonal net
    int quantity;
    int hp, hptot; // current health of the last unit in the stack, total health of the stack
    bool retaliation=true;
    public:
    UStack(const char *uname, int attack=1, int defense=1, int mindamage=1, int maxdamage=2, int speed=4, int health=15, 
          bool shooter=false, bool flyer=false,
           int quantity=10, int hexes15=1, int hexes11=1)
    {
        strcpy(this->unitName, uname);
        this->attack=attack;
        this->defense=defense;
        this->mindamage=mindamage;
        this->maxdamage=maxdamage;
        this->speed=speed;
        this->quantity=quantity;
        this->health=health;
        this->shooter=shooter;
        this->flyer=flyer;
        
        this->hp=health; 
        this->hptot=health*quantity;
        this->hexes15=hexes15; 
        this->hexes11=hexes11; 
    }
    
    UStack PrintU()
    {
      printf("\nAny %s\n attack=%i\n defense=%i\n damage=%i-%i\n speed=%i\n health=:%i\n", unitName, attack, defense, mindamage, maxdamage, speed, health);
      printf(shooter ? " True shooter\n" : " Not shooter at all\n");
      printf(flyer ? " True flyer\n" : " Not flyer at all\n");
      printf("These concrete %i %ss\n hp=%i/%i, total %i of %i \n position=(%i, %i)\n", quantity, unitName, hp, health, hptot, hptot-hp+health, hexes15, hexes11);
      printf(retaliation ? " Can retaliate\n" : " No more retaliations in this round\n");
      
    }
    friend bool operator>(const UStack & s1, const UStack & s2) {
      if(s1.quantity*(s1.maxdamage+s1.mindamage)*(1+(s1.attack-s2.defense)*0.05) > s2.quantity*(s2.maxdamage+s2.mindamage)*(1+(s2.attack-s1.defense)*0.05))
        return true;
      else
        return false;
    }
    
    friend bool operator<(const UStack & s1, const UStack & s2) {
      if(s1.quantity*(s1.maxdamage+s1.mindamage)*(1+(s1.attack-s2.defense)*0.05) < s2.quantity*(s2.maxdamage+s2.mindamage)*(1+(s2.attack-s1.defense)*0.05))
        return true;
      else
        return false;
    }
    
    friend bool operator>=(const UStack & s1, const UStack & s2) {
      if(s1.quantity*(s1.maxdamage+s1.mindamage)*(1+(s1.attack-s2.defense)*0.05) >= s2.quantity*(s2.maxdamage+s2.mindamage)*(1+(s2.attack-s1.defense)*0.05))
        return true;
      else
        return false;
    }
    friend bool operator<=(const UStack & s1, const UStack & s2) {
      if(s1.quantity*(s1.maxdamage+s1.mindamage)*(1+(s1.attack-s2.defense)*0.05) <= s2.quantity*(s2.maxdamage+s2.mindamage)*(1+(s2.attack-s1.defense)*0.05))
        return true;
      else
        return false;
    }
    friend bool operator!=(const UStack & s1, const UStack & s2) {
      if(s1.quantity*(s1.maxdamage+s1.mindamage)*(1+(s1.attack-s2.defense)*0.05) != s2.quantity*(s2.maxdamage+s2.mindamage)*(1+(s2.attack-s1.defense)*0.05))
        return true;
      else
        return false;
    }
    
    virtual bool CanMove (const UStack & s)
    {
      return s.speed>0;
    }
    UStack Move(int x, int y)
    {
      if (!CanMove(this))
      {
        printf("Stack %s is immobile now\n", s.unitName);
        return *this;
      }
      if(x > 15 || y > 11 || x <=0 || y<=0)
      {
        printf("Units Can not move outside the battlefield\n");
        return *this;
      }
      int distance=(hexes11-y)*(hexes11-y)+(hexes15-x)*(hexes15-x);
      int sqspeed=speed*speed;
      if(distance >= sqspeed)
      {
        printf("The speed of %s is insufficient to move where you want\n", unitName);
        return *this;
      }
      printf("Unit %s  has succesfully moved from (%i,%i) to (%i,%i)\n", unitName, hexes15, hexes11, x, y);
      s.hexes11=y;
      s.hexes15=x;
      return *this;
    }
    virtual UStack* Clone() const
    {
    	return new UStack(this->unitName,  this->attack,this->defense, 
    	this->mindamage, this->maxdamage, this->speed, this->health,
    	this->shooter, this-> flyer,
    	this->quantity, this->hexes15, this->hexes11);
    }
    virtual ~UStack()
    {
    	
    }
   
};

class Hero:public UStack
{
  public:
  Hero(char *uname, int attack=1, int defense=1): UStack(uname, attack, defense,attack+defense,(attack+defense)*3, 0, 1, 
             true, false,
             1, 0, 0){}
  bool CanMove(const Hero & s)
  {
    return false;
  }
   virtual Hero* Clone() 
   {
  	return new Hero(this->unitName,  this->attack,this->defense);
   }
  ~Hero()
  {
  	
  }
};

class Battlefield
{
  private:
  int xsize=11;
  int ysize=15;
  UStack *Objects[30];
  int number=0;
  public:
  Battlefield()
  {
  }
  Battlefield Print()
  {
  	printf("Number of Stacks and heroes is %i\n", number);
  	for(int i=0;i<number;i++)
  	{
  	 printf("\nObject N %i", i+1);
  	 Objects[i]->PrintU();
  	}
  	printf("That's the end of the battlefield with %i objects\n", number);
  }
  
  Battlefield Add(UStack *S)
  {
  	Objects[number]=S->Clone();
  	number++;
  	printf("Unit number %i has been added succesfully\n", number);
  }
  
  int Delete(int n)
  {
  	 delete(Objects[n]); 
  	 for(int i=n; i<number; i++)
  	 Objects[i]=Objects[i+1];
  	 number--;
  	 printf("Object number %i has been succesfully deleted", n);
  	 return n;
  }
  //  destruction
};
class StandBf
{
 private:
 list<UStack*> Bf;
 public:
 StandBf()
 {
 }
 void Print()
 {
 	list<UStack*>::iterator it1 = Bf.begin();
 	int i=0;
 	 while(it1 != Bf.end())
    {
     
     printf("Unit Number is %i starting with zero (or %i stating with 1)", i++, i); 
     (*it1)->PrintU();
     it1++;
    }
    printf("That's the end of the list");
 }
  void Add(const UStack& S)
  {
  	Bf.push_front(S.Clone());
  }
  void Delete(int n)
  {
  	list<UStack*>::iterator it2 = Bf.begin();
  	int i = 0;
  	for(;it2 != Bf.end() ;it2++,i++)
  		if(i==n)
  	   {
  		   //delete *it2;
  		   Bf.erase(it2);
  	   }
  }
  ~StandBf()
 { 
  list<UStack*>::iterator it2 = Bf.end();
  while(it2!=Bf.begin())
  {
   delete(*it2);
   it2=it2--;
  }
  Bf.clear();
 }
};

int main() {
    char uname1[30]="gargoyle";
    char uname2[30]="golem";
    UStack G1(uname1, 8, 7, 2, 3, 4, 15, 
             false, true,
             11, 1, 10);
    //PrintU();
    UStack G2(uname2, 8, 5, 2, 3, 4, 35, 
             false, false,
             156, 1, 7);
    //G2.PrintU();
    //fputs((G2<=G1) ? "true" : "false", stdout);
    char H1name[20]="Solmir";
    Hero H1(H1name, 8, 7);
    //H1.PrintU();
    StandBf B;
    
    /* H1.Move(H1,1,2);
    G1.Move(G1,2, 11);
    G2.Move(G1,2,9); */
    
    //B.Print();
  for(int i=0;i<10;i++)
   {
   	B.Add(G1);
   	//B.Add(G2);
   	//B.Add(H1);
   }
   
  
   B.Print();
   //printf("123\n");
   B.~StandBf();
    
}


             