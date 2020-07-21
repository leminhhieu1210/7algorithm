#include "PageReplacement.h" //goi thu vien

int displayMenu() { 
	int choice;
	//hien thi menu cho nguoi dung chon
	cout << "1. 4 thuat toan lap lich CPU.\n";
	cout << "2. 3 thuat toan phan trang.\n";
	cout << "3. Thoat.\n";
	cout << "Lua chon cua ban: ";
	
	cin >> choice;
	
	return choice;
}

void yourChoise() {
	int n;
	char c;
	Scheduling s;
	PageReplacement p;
	//chay chuong trinh
	do {
		n = displayMenu();
		switch(n)
		{
			case 1:
				s.input();
				
				s.solveFCFS();
				s.solveSJN();
				s.solveSRN();
				s.solveRR();
				
				cout << "Xong\n";
				break;
			case 2:
				p.solveFIFO();
				p.solveOPT();
				p.solveLRU();
				
				cout << "Xong\n";
				break;
			default:
				goto thoat;
		} 
		
		cout << "Ban co muon tiep tuc? Y/N\n";
		cin >> c;
		if (c != 'Y' && c != 'y') goto thoat;
		else system("cls");
		
	} while (n == 1 || n == 2);
	
	thoat:
		system("cls");
		cout << "\nSee you later!\n";
		fo.close();
}

int main() {	
	yourChoise();
	
	return 0;
}

