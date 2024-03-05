#include <iostream>
#include <string>
#include <string_view>
#include <array>
#include <random>
#include <ctime>



int getrandomnumber(int min, int max)
{
	static std::mt19937 mt{ static_cast<std::mt19937::result_type>(std::time(nullptr)) };
	static std::uniform_int_distribution randomnumber{ min,max };
	return randomnumber(mt);
}

class Creature
{
protected:
	std::string mname{};
	char msymbol{};
	int mhealth{};
	int mdamage{};
	int mgold{};

public:

	Creature(std::string_view name, char symbol, int health, int damage, int gold)
		: mname{ name }, msymbol{ symbol }, mhealth{ health }, mdamage{ damage }, 
		mgold {gold}
	{}
	const std::string& getname() { return mname; }
	char getsymbol() { return msymbol; }
	int gethealth() { return mhealth; }
	int getdamage() { return mdamage; }
	int getgold() { return mgold; }

	void reducehealth(int healthreduced)
	{
		mhealth -=healthreduced;
	}

	void addhealth(int healthadded)
	{
		mhealth += healthadded;
	}

	void adddamage(int damageadded)
	{
		mdamage += damageadded;
	}

	bool isdead()
	{
		if (mhealth <= 0)
		return true;
		return false;
	}

	void addgold(int goldgot)
	{
		mgold += goldgot;
	}
};

class Potion
{
public:
	enum class Type
	{
		health,
		strength,
		poison,
		maxtypes
	};

	struct Size
	{
		std::string effectlevel{};
		int effectsize{};
	};



private:
	Type mtype{};
	Size msize{};
public:

	Potion(Type type, Size size)
		: mtype{ type }, msize{ size }
	{}

	Potion() {}

	static Potion getrandompotion()
	{
		int randomnumber{ getrandomnumber(0, static_cast<int>(Type::maxtypes) - 1) };
		Potion potion{};
		potion.mtype = static_cast<Type>(randomnumber);

		int randomeffect{ getrandomnumber(0, 2) };

		switch (randomeffect)
		{
		case 0:
			potion.msize.effectlevel = "small";
			break;
		case 1:
			potion.msize.effectlevel = "medium";
			break;
		case 2:
			potion.msize.effectlevel = "large";
		}

		if (potion.mtype == Type::health)
		{
			if (randomeffect == 0 || randomeffect == 1)
				potion.msize.effectsize = 2;
			else
				potion.msize.effectsize = 5;
		}
		else if (potion.mtype == Type::strength)
			potion.msize.effectsize = 1;
		else
			potion.msize.effectsize = -1;

		return potion;
	}

	friend std::ostream& operator<< (std::ostream& out, Type type)
	{
		if (type == Type::health)
			out << "health";
		else if (type == Type::strength)
			out << "strength";
		else
			out << "poison";
		return out;
	}

	Potion::Type getpotiontype()
	{
		return mtype;
	}

	int getpotionsize()
	{
		return msize.effectsize;
	}

	const std::string& getpotionlevel()
	{
		return msize.effectlevel;
	}




};

class Player: public Creature
{
private:
	int mlevel{ 1 };

public:
	Player(std::string_view name, char symbol='@', int health=10, int damage=1, int gold=0)
		: Creature{name, symbol, health, damage, gold}
	{}

	void levelup()
	{
		++mlevel;
		++mdamage;
	}

	int getlevel() { return mlevel; }
	bool haswon()
	{
		if (mlevel >= 20)
			return true;
		return false;
	}
	
	void drinkpotion(Potion& potion)
	{
		
		if (potion.getpotiontype() == Potion::Type::health)
		{
			addhealth(potion.getpotionsize());
		}
		else if (potion.getpotiontype() == Potion::Type::strength)
		{
			adddamage(potion.getpotionsize());
		}
		else
		{
			reducehealth(potion.getpotionsize());
		}

	}
	
};

class Monster: public Creature
{
public:
	enum class Type
	{
		dragon, 
		orc, 
		slime,
		maxtypes
	};

	Monster(Type type)
		: Creature{ getdefaultcreature(type) }
	{}

	static Monster getrandommonster()
	{
		int randomnumber{ getrandomnumber(0, static_cast<int>(Type::maxtypes) - 1 )};
		return Monster{ static_cast<Type>(randomnumber)};
	}
	

	

private:
	static const Creature& getdefaultcreature(Type type)
	{
		static const std::array<Creature, static_cast<std::size_t>(Type::maxtypes)> monsterData{
		  { { "dragon", 'D', 20, 4, 100 },
			{ "orc", 'o', 4, 2, 25 },
			{ "slime", 's', 1, 1, 10 } }
		};

		return monsterData.at(static_cast<std::size_t>(type));
	}
	
};



void attackmonster(Monster& monster, Player& player);
void attackplayer(Monster& monster, Player& player);

void fightmonster(Monster& monster, Player& player)
{
	if (player.gethealth() > 0)
	{
		char fight{};
		std::cout << "do you wanna run (r) or fight (f) ? ";
		std::cin >> fight;
		if (fight == 'r')
		{
			int escape{ getrandomnumber(0,1) };
			if (escape)
			{
				player.levelup();
				std::cout << "you managed to escape and now youre on level " << player.getlevel() << ".\n";
			}
			else
			{
				std::cout << "you did not manage to escape\n";
				attackplayer(monster, player);
			}
		}
		else if(fight=='f')
		{
			attackmonster(monster, player);
		}
	}

	
}



void attackmonster(Monster& monster, Player& player)
{
	monster.reducehealth(player.getdamage());
	std::cout << "you managed to attack the monster and now the monster health is " << monster.gethealth() << '\n';
	if (monster.gethealth() <= 0)
	{
		player.addgold(monster.getgold());
		player.levelup();
		
		std::cout << "now the monster died ";
		int potionchance = getrandomnumber(1, 10);
		if (potionchance <4)
		{
			Potion potion{ Potion::getrandompotion() };
			std::cout << "you found a mythical potion! do you want to drink it? [y/n]: ";
			char check{};
			std::cin >> check;
			if (check == 'y')
			{
				player.drinkpotion(potion);
				std::cout << "you drank a " << potion.getpotionlevel() << " potion of " << potion.getpotiontype()<<'\n';
				if (player.gethealth() < 0)
				{
					std::cout << "you died at level " << player.getlevel() << " and with " << player.getgold() << " gold.\n";
					std::cout << "too bad you can't take it with you!\n";
				}
			}
			
		}
		std::cout << "you got " << monster.getgold() << " gold and leveled up to level " << player.getlevel() << '\n';
	}
	else
	{
		attackplayer(monster, player);
	}
}

void attackplayer(Monster& monster, Player& player)
{
	player.reducehealth(monster.getdamage());
	std::cout << "the monster attacked you and now you have " << player.gethealth() << " health.\n";
	fightmonster(monster, player);
}

int main()
{
	
	
	std::cout << "enter your name: ";
	std::string name{};
	std::cin >> name;
	Player player{ name };

	while (player.gethealth() > 0 && player.getlevel() < 20)
	{
		Monster monster{ Monster::getrandommonster() };
		std::cout << "you have encountered a " << monster.getname() << ".\n";
		fightmonster(monster, player);
	}
	if (player.haswon())
		std::cout << "you won with "<< player.getgold()<< " gold. congrats!\n";
	else
	{
		std::cout << "you died at level " << player.getlevel() << " and with " << player.getgold() << " gold.\n";
		std::cout << "too bad you cant take it with you!\n";

	}
}

