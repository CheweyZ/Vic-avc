
int sectionNum = 1;
int sleepAmount = 10000;

int main()
{
	//set up
	init();

	//repeating section
	while(sectionNum <= 5)
	{
		if (sectionNum == 1)
		{ //before gate
			/*call door*/
			/*call follow line*/
		}
		else if (sectionNum == 2)
		{ //follow line
			/*call follow line*/
			//sharpLineTest();
		}
		else if (sectionNum == 3)
		{ //follow sharp line
			//sharpLine();
		}
		else if (sectionNum == 4)
		{ //follow maze before gate
			// call redTest(int scanning_row);
			//mazeSection();
		}
		else if (sectionNum == 5)
		{ //follow maze after gate
			//mazeSection();
		}
		sleep1(0,sleepAmount);
	}
	return 0;
}
