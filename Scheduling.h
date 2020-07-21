#include <bits/stdc++.h>
#define pb push_back
#define amount 10

using namespace std;

ofstream fo("output.txt");

class Scheduling {
	private:
		int n, q;
		int sjn[amount][6]; 
		int ArrivalTime[amount], BurstTime[amount];					
	public:
		Scheduling(); //ham khoi tao
		void input(); //ham nhap
		
		int number_length(int); //ham dem so chu so
		void drawMinus(int); //ham ve dau "-", "|"
		void drawSpace(int); //ham in dau space
		void drawTable(int, int [], int [], int); //ham ve so do
		
		void solveFCFS2(int, int [], int [], int [], int [], int []); //ham xu ly FCFS: thoi gian cho, thoi gian luu
		void solveFCFS(); //ham xu ly FCFS
		
		void sortArrival(int , int [][6]); //sap xep thoi gian den
		void solveSJN2(int , int [][6]); //ham xu ly SJN: thoi gian cho, thoi gian luu
		void solveSJN(); //ham xu ly SJN
		
		void solveSRN2(int, int [], int [], int [], int [], int [], int [], double&, double&, int&); //ham xu ly SRN: thoi gian cho, thoi gian luu,...	
		void solveSRN(); //ham xu ly SRN
		
		void solveRR2(int, int, int [], int [], int [], int [], int [], int [], int&, int&, int&); //ham xu ly RR: thoi gian cho, thoi gian luu,...
		void solveRR(); //ham xu ly RR
};

Scheduling::Scheduling() {
	n = q = 0;
}

void Scheduling::input() {
	ifstream fi("input1.txt");
	
	vector<int> temp;
	int id = -1, x;
	
	while (!fi.eof()) //doc file
	{
		fi >> x;
		temp.pb(x);
		id++;
	}	
	
	n = id/2;
	for(int i = 0; i < n; ++i) 
	{ 
		sjn[i][0] = i;  //sjn[i][0]: chi so qua trinh
		sjn[i][1] = temp[i];  //sjn[i][1], ArrivalTime[i]: thoi gian xuat hien
		ArrivalTime[i] = temp[i];
	}
	
	int id2 = 0;
	for(int i = n; i < id - 1; ++i) 
	{ 
		sjn[id2][2] = temp[i];  //sjn[id2][2], BurstTime[id2]: thoi gian chay
		BurstTime[id2] = temp[i]; 
		id2++;
	}
	q = temp[id - 1]; //q trong Round Robin

	fi.close();
}

int Scheduling::number_length(int n) { //tinh do dai chu so
	if (n) return log10((double)n) + 1;
	return 1;
}

void Scheduling::drawMinus(int t) { //ve |--------|
	fo << "|";
	for (int i = 1; i < t; ++i) fo << "-";
}

void Scheduling::drawSpace(int t) { //ve t dau space
	for (int i = 1; i <= t; ++i) fo << " ";
}

void Scheduling::drawTable(int n, int p1[], int p2[], int option) {
	if (option == 1) //option = 1 -> ve FCFS
	{
		int space;
		fo << endl;
		for (int i = 0; i < n - 2; ++i) 
		{
			space = 1;
			space += number_length(i); //space = do dai cac ki tu P0, P1, P2,...
			fo << "P" << i + 1; //in chi so: P0     P1    P2
			space = p2[i + 1] - p2[i] - space;
			if (p2[i + 1] - p2[i] < 2) space = 1;
			drawSpace(space); //in so luong dau space
		}
		fo << "P" << n - 1 << endl;
		
		for (int i = 0; i < n - 1; ++i) //in: |-------|---|
		{
			space = p2[i + 1] - p2[i];
			drawMinus(space);
			if (space < 2) fo << "  "; //truong hop suy bien
		}
		fo << "|" << endl;
		
		for (int i = 0; i < n; ++i) //in gia tri: 0		2		5
		{
			fo << p2[i];
			space = number_length(p2[i]); //tinh so luong dau space
			if (space == 1 && p2[i + 1] - p2[i] == 1) space = 2;
			else
			{
				space = p2[i + 1] - p2[i] - space;
				if (space < 2) space = 1;
			}
			drawSpace(space); //in so luong dau space
		}
		fo << endl;
	}
	else if (option == 4) //option = 4 -> ve RR
	{
		fo << endl;
		for (int i = 0; i < n - 1; ++i) 
			fo << "P" << p1[i] + 1 << "\t"; //in: P0     P1    P2
		fo << "P" << p1[n - 1] + 1 << endl;
		
		for (int i = 0; i < n; ++i) //in: |-------|---|
			drawMinus(4);
		fo << "|" << endl;
		
		for (int i = 0; i <= n; ++i) //in gia tri: 0		2		5
			fo << p2[i] << "\t";
		fo << endl;
	}
	else //ve SJN, SRN
	{
		int space;
		fo << endl;
		for (int i = 0; i < n - 1; ++i) 
		{
			space = 1;
			space += number_length(i); //space = do dai xau P0, P1, P2,...
			fo << "P" << p1[i] + 1; //in: P0     P1    P2
			space = p2[i + 1] - p2[i] - space;
			if (p2[i + 1] - p2[i] < 2) space = 1;
			drawSpace(space); //in space giua: P0    P1
		}
		fo << "P" << p1[n - 1] + 1 << endl;
		
		for (int i = 0; i < n; ++i) //in: |-------|---|
		{
			space = p2[i + 1] - p2[i];
			drawMinus(space);
			if (space < 2) fo << "  ";
		}
		fo << "|" << endl;
		
		for (int i = 0; i <= n; ++i) //in gia tri: 0		2		5
		{
			fo << p2[i];
			space = number_length(p2[i]); //tinh so luong dau space
			if (space == 1 && p2[i + 1] - p2[i] == 1) space = 2;
			else
			{
				space = p2[i + 1] - p2[i] - space;
				if (space < 2) space = 1;
			}
			drawSpace(space); //in so luong dau space
			if (!space) //truong hop suy bien
			{
				i++;
				drawSpace(p2[i + 1] - p2[i] - space + 1); //in so luong dau space
			}
		}
		fo << endl;
	}
}

void Scheduling::solveFCFS2(int n, int art[], int bt[], int wt[], int ts[], int st[]) { 
	// st[]: mang luu gia tri thoi gian xu ly
	st[0] = 0; wt[0] = 0;
	
//	fo << "\nThoi gian cho:\n"; //in ra file
//	fo << "P" << 1 << " = " << st[0] << " - " << art[0] << " = " << wt[0] << endl;
	for (int i = 1; i < n ; i++) 
	{  
		st[i] = st[i - 1] + bt[i - 1]; //thoi gian xu ly hien tai = thoi gian xu ly truoc + thoi gian chay truoc 
		wt[i] = st[i] - art[i]; //thoi gian cho = thoi gian xu ly - thoi gian xuat hien
		if (wt[i] < 0) wt[i] = 0;
//		fo << "P" << i + 1 << " = " << st[i] << " - " << art[i] << " = " << wt[i] << endl;
	} 
	st[n] = st[n - 1] + bt[n - 1];
	
//	fo << "\nThoi gian luu:\n"; //in ra file
	for (int i = 0; i < n; i++) 
	{
		ts[i] = bt[i] + wt[i]; //thoi gian luu = thoi gian cho + thoi gian chay
//		fo << "P" << i + 1 << " = " << bt[i] << " + " << wt[i] << " = " << ts[i] << endl;
	}
} 

void Scheduling::solveFCFS() {
	fo << "\n\n~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~4 thuat toan lap lich CPU~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\n\n";
	fo << "\n**Phuong Phap FCFS:**\n";

	int p1[amount] = {}, p2[amount], wt[amount], ts[amount]; 
	solveFCFS2(n, ArrivalTime, BurstTime, wt, ts, p2);

	int sum_wt = 0, sum_ts = 0; //tong thoi gian cho, tong thoi gian luu
	p2[0] = 0; //mang luu cac gia tri thoi gian phuc vu
	for (int i = 0 ; i < n ; i++) 
	{ 
		sum_wt += wt[i]; //tinh tong thoi gian cho 
		sum_ts += ts[i]; //tinh tong thoi gian luu
	}
	drawTable(n + 1, p1, p2, 1); //ve so do
	
	fo << "\nThoi gian cho:\n"; //in ra file
	for (int i = 0; i < n; ++i)
		fo << "P" << i + 1 << " = " << wt[i] << endl;
	fo << "Thoi gian cho trung binh = " << (float)sum_wt / n << endl; 
	
	fo << "\nThoi gian luu:\n"; //in ra file
	for (int i = 0; i < n; ++i)
		fo << "P" << i + 1 << " = " << ts[i] << endl;
	fo << "Thoi gian luu trung binh = " << (float)sum_ts / n << endl; 		
}

void Scheduling::sortArrival(int n, int sjn[][6]) { 
	for(int i = 0; i < n; ++i) 
	{ 
		for(int j = 0; j < n - i - 1; ++j) 
		{ 
			if(sjn[j][1] > sjn[j+1][1]) //sap xep dua theo thoi gian den
			{ 
				for(int k = 0; k < 5; ++k) //doi cho thoi gian chay,... theo thoi gian den
					swap(sjn[j][k], sjn[j+1][k]); 
			} 
		} 
	} 
} 

void Scheduling::solveSJN2(int n, int sjn[][6]) { 
	int t, id; 
	sjn[0][3] = sjn[0][1] + sjn[0][2]; //sjn[0][3] = thoi gian xu ly =  thoi gian den + thoi gian xuat hien
	sjn[0][5] = sjn[0][3] - sjn[0][1]; //khoi gan thoi gian luu thu 0
	sjn[0][4] = sjn[0][5] - sjn[0][2]; //khoi gan thoi gian cho thu 0
	
	for(int i = 1; i < n; ++i) 
	{ 
		t = sjn[i - 1][3]; //t = thoi gian den truoc + thoi gian xuat hien truoc
		int Min = sjn[i][2]; //Min = thoi gian cho thu i
		for(int j = i; j < n; ++j) 
		{ 
			if(t >= sjn[j][1] && Min >= sjn[j][2]) //neu thoi gian xu ly = nhau -> chon thoi gian chay Min
			{ 
				Min = sjn[j][2]; 
				id = j; //lay vi tri
			} 
		} 
		sjn[id][3] = t + sjn[id][2]; //thoi gian xu ly
		sjn[id][5] = sjn[id][3] - sjn[id][1];  //thoi gian luu
		sjn[id][4] = sjn[id][5] - sjn[id][2];  //thoi gian cho
		for(int k = 0; k < 6; ++k) //doi vi tri
			swap(sjn[id][k], sjn[i][k]); 
	} 
} 

void Scheduling::solveSJN() {
	fo << "\n\n**Phuong Phap SJN:**\n";
	
	int p3[n];
	for (int i = 0; i < n; ++i) p3[i] = sjn[i][2];
	
	sortArrival(n, sjn); //goi ham xu ly
	solveSJN2(n, sjn); 
	
	int sum_wt = 0, sum_ts = 0; //tong thoi gian cho, tong thoi gian luu
	int p1[n], p2[n + 1]; //p1: luu chi so id cua P1, P2, P4,... | p2: luu gia tri thoi gian xu ly
	for (int i = 0; i < n; ++i)
	{
		p1[i] = sjn[i][0]; //p1: luu chi so id P1, P2, P4,...
		p2[i] = sjn[i][4] + sjn[i][1]; //p2: luu gia tri thoi gian xu ly
		
		sum_wt += sjn[i][4]; //tong thoi gian cho 
		sum_ts += sjn[i][5]; //tong thoi gian luu
	}
	p2[n] = p2[n - 1] + p3[p1[n - 1]];
	drawTable(n, p1, p2, 2); //ve so do
	 
	fo << "\nThoi gian cho:\n"; //in ra file
	for (int i = 0; i < n; ++i)
		fo << "P" << sjn[i][0] + 1 << " = " << sjn[i][4] << endl;
	fo << "Thoi gian cho trung binh = " << (float)sum_wt / n << endl; 
	
	fo << "\nThoi gian luu:\n"; //in ra file
	for (int i = 0; i < n; ++i)
		fo << "P" <<  sjn[i][0] + 1 << " = " << sjn[i][5] << endl;
	fo << "Thoi gian luu trung binh = " << (float)sum_ts / n << endl; 
}

void Scheduling::solveSRN2(int n, int a[], int b[], int p1[], int p2[], 
					   int wt[], int ts[], double &sum_wt, double &sum_ts, int &id) {
	int x[amount], Min, count = 0, time;
	double end;
	
	for(int i = 0; i < n; i++) x[i] = b[i];	//sao luu thoi gian chay
	
	b[9] = 9999, time = 0, id = 1, sum_wt = 0, sum_ts = 0;
	
	while(count != n)
	{
		Min = 9; //khoi gan Min = chi so id lon nhat
		for(int i = 0; i < n; i++)
		{
			//lay id cua tien trinh nho nhat dang chay
			if(a[i] <= time && b[i] < b[Min] && b[i] > 0) Min = i;
		}
		b[Min]--; //thoi gian chay giam dan
		if (Min != p1[id - 1])
		{
			p1[id] = Min; //p1: luu chi so id cua P1, P2, P4,...
			p2[id] = time; //p2: luu gia tri thoi gian xu ly
			id++; //chi so 2 mang p1, p2 tang 1.
		}
		if(!b[Min]) //neu thoi gian chay == 0
		{
			count++; //bien dem tang
			end = time + 1; //thoi gian xu ly cuoi cung cua tien trinh
			
			wt[Min] = end - a[Min] - x[Min]; //thoi gian cho = thoi gian xu ly cuoi cung - thoi gian xuat hien - thoi gian chay
			sum_wt += wt[Min]; //tong thoi gian cho
			
			ts[Min] = end - a[Min]; //thoi gian luu = thoi gian xu ly cuoi cung - thoi gian xuat hien
			sum_ts += ts[Min]; //tong thoi gian luu
		}
		time++; //thoi gian xu ly tang 1
	} 
	p2[id] = end;
}
 
void Scheduling::solveSRN() {
	int p1[amount] = {}, p2[amount] = {}, wt[amount], ts[amount], b[amount], id;
	double sum_wt = 0, sum_ts = 0;
	
	fo << "\n\n**Phuong Phap SRN:**\n";
	for (int i = 0; i < n; ++i) b[i] = BurstTime[i]; //sao luu thoi gian chay
	solveSRN2(n, ArrivalTime, b, p1, p2, wt, ts, sum_wt, sum_ts, id); //goi ham xu ly
	drawTable(id, p1, p2, 3); //ve so do
	
	fo << "\nThoi gian cho:\n"; //in ra file
	for (int i = 0; i < n; ++i) 
		fo << "P" << i + 1 << " = " << wt[i] << endl;
	fo << "Thoi gian cho trung binh = " << sum_wt / n << endl;
	
	fo << "\nThoi gian luu:\n"; //in ra file
	for (int i = 0; i < n; ++i) 
		fo << "P" << i + 1 << " = " << ts[i] << endl;
	fo << "Thoi gian luu trung binh = " << sum_ts / n << endl;
}

void Scheduling::solveRR2(int n, int q, int a[], int b[], int wt[], int ts[], int p1[], int p2[], int &id,
		      int &sum_wt, int &sum_ts) {
	int i, x, sum = 0, check = 0;
	int temp[amount];
	
	for(i = 0; i < n; i++) temp[i] = b[i]; //sao luu thoi gian chay
	
	sum = i = 0; id = 1, p2[0] = 0, x = n, sum_wt = 0, sum_ts = 0;
	while(x)
	{
	    if(temp[i] <= q && temp[i] > 0)
	    {
	          sum += temp[i]; //thoi gian xu ly tren so do
	          temp[i] = 0; //thoi gian chay = 0
	          check = 1;
	    }
	    else if(temp[i] > 0)
	    {
	          temp[i] = temp[i] - q; //thoi gian chay giam q
	          sum += q; //thoi gian xu ly tang them q
	    }
		
		if (sum != p2[id - 1]) 
		{
			p1[id] = i; //p1: luu chi so id cua P1, P2, P4,...
			p2[id] = sum; //p2: luu gia tri thoi gian xu ly
			id++;
		}		
	    if(temp[i] == 0 && check == 1)
        {
			x--; check = 0;			
			wt[i] = sum - a[i] - b[i]; //thoi gian cho = thoi gian xu ly cuoi cung - thoi gian xuat hien - thoi gian chay
			sum_wt += wt[i]; //tong thoi gian cho
			ts[i] = sum - a[i]; //thoi gian luu = thoi gian xu ly cuoi cung - thoi gian xuat hien
			sum_ts += ts[i]; //tong thoi gian luu	
	    }
		
		if (i == n - 1) i = 0;
	    else if(a[i + 1] <= sum) i++; //thoi gian xuat hien <= thoi gian xu ly --> chuyen den tien trinh tiep theo
	    else i = 0; //chay lai tu dau
	}
	
	for (int i = 0; i < id - 1; ++i) p1[i] = p1[i + 1]; //cho mang p1 xuat phat tu 0
	id -= 1; p2[0] = 0; 
}

void Scheduling::solveRR() {
	int id, sum_wt = 0, sum_ts = 0;
	int wt[amount], ts[amount], p1[3*amount], p2[3*amount];
	
	fo << "\n\n**Phuong Phap RR:**\n";
	
	solveRR2(n, q, ArrivalTime, BurstTime, wt, ts, p1, p2, id, sum_wt, sum_ts); //goi ham xu ly
	drawTable(id, p1, p2, 4); //ve so do
	
	fo << "\nThoi gian cho:\n"; //in ra file
	for (int i = 0; i < n; ++i) 
		fo << "P" << i + 1 << " = " << wt[i] << endl;
	fo << "Thoi gian cho trung binh = " << (float)sum_wt / n << endl;
	
	fo << "\nThoi gian luu:\n"; //in ra file
	for (int i = 0; i < n; ++i) 
		fo << "P" << i + 1 << " = " << ts[i] << endl;
	fo << "Thoi gian luu trung binh = " << (float)sum_ts / n << endl;	
}
