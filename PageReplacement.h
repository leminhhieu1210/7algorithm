#include"Scheduling.h" //goi thu vien
#define amountMat 50

class PageReplacement {
	private:
		int n, f, error = 0;
		vector<vector<int>> matTb; //vector de ve bang
		vector<int> err; //vector chua loi
		vector<int> tr; //vector chua so trang	
		vector<int> Opt;
	
	public:			
		PageReplacement(); //ham khoi tao
		void input(); //ham nhap
			
		void setTb(); //ham khoi tao bang
		void backupCot(int, int, int, int);	//sao luu gia tri cua cot truoc
		void drawTable(); // hien thi bang			
		
		void solveFIFO2();		
		void solveFIFO(); //ham xu ly FIFO				
		
		int check(int); //kiem tra trang ton tai hay khong	
		int find(int, int); //tim vi tri xuat hien xa nhat trong tuong lai
		int solveOPT2(); 
		void solveOPT(); //ham xu ly OPT
		
		void solveLRU2();				
		void solveLRU(); //ham xu ly LRU	
};

PageReplacement::PageReplacement() {
	n = f = error = 0;
}

void PageReplacement::setTb() {		
	fill(err.begin(), err.end(), -1); //khoi tao mang = cac gia tri -1
	for (int i = 0; i < f + 1; ++i)
		fill(matTb[i].begin(), matTb[i].end(), -1);
		
	for(int i = 0; i < n; ++i) matTb[0][i] = tr[i]; //hang thu 0 chua cac gia tri dau vao
	for(int j = 1; j <= f; ++j)
	{
		for(int i = j; i <= f; ++i) //f cot dau tien gan luon ket qua cua f trang dau tien
			matTb[j][i - 1] = tr[j - 1];
	}
}

void PageReplacement::backupCot(int cot, int trc, int sau, int k) {
	for(int i = 1; i <= k; ++i) matTb[i][cot] = matTb[i][cot - 1]; //gia tri truoc = gia tri sau
	for(int i = 1; i <= k; ++i)
	{
		if(matTb[i][cot - 1] == trc) 
		{
			matTb[i][cot] = sau; //gia tri truoc = gia tri sau
			return;
		}
	}
}

void PageReplacement::drawTable() {
	for (int k = 0; k < n; ++k) fo << "----"; fo << endl; //ve "-----"
	for(int i = 0; i < f + 1; ++i)
	{
		for(int j = 0; j < n; ++j)
		{
			if (matTb[i][j] == -1) fo << "  | "; 
			else fo << matTb[i][j] << " | "; //ve dau " | "
		}
		fo << endl;
		for (int k = 0; k < n; ++k) fo << "----";
		fo << endl;
	}
	for (int i = 0; i < n; ++i) //ve bang loi
	{
		if (i == err[i]) fo << "X | "; //ve dau " X "
		else fo << "  | ";
	}
	fo << endl;
	for (int k = 0; k < n; ++k) fo << "----"; fo << endl;
}

void PageReplacement::input() {
	ifstream fi("input2.txt");	
	
	vector<int> temp;
	int x, id;	
	
	while(!fi.eof()) //doc file
	{
		fi >> x;
		temp.push_back(x);
		id++;
	} 
	
	for (int i = 0; i < id - 2; ++i) tr.push_back(temp[i]);
	n = tr.size();
	f = temp[id - 2]; //doc so khung
	
	err.resize(n + 10); //cap phat vector
	matTb.resize(f + 10);
	for (int i = 0; i < f + 1; ++i)
		matTb.at(i).resize(n + 10);
		
	fi.close();
}

void PageReplacement::solveFIFO2() {
	queue<int> Q; //Q de luu cac trang
	unordered_set<int> F;
	int val;
		
	for(int i = 0; i < n; i++)
	{
		if(F.size() < f) //ktra da chay het den cuoi chua
		{
			if(F.find(tr[i]) == F.end()) 			
			{
				Q.push(tr[i]);
				F.insert(tr[i]);
				err[i] = i; //danh dau vi tri loi
				error++; //so loi tang 1
			}	
		}
		else
		{
			if(F.find(tr[i]) == F.end()) //Tim neu khong thay trang hien tai thi them vao
			{
				val = Q.front(); 
				Q.pop(); //lay gia tri roi xoa trang
				Q.push(tr[i]); //Q day trang hien tai vao cuoi
				
				F.erase(val); //xoa			
				F.insert(tr[i]); //F luu trang hien tai
							
				err[i] = i; //danh dau vi tri error
				error++; //so loi tang 1
			}
			backupCot(i, val, tr[i], f); //chen gia tri vao bang
		}
	}
}

void PageReplacement::solveFIFO() {	
	fo << "\n\n~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~3 thuat toan thay the trang~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\n\n";	
	fo << "\n**Phuong Phap Thay The Trang FIFO:**\n\n";
	input();

	setTb(); //khoi tao bang
	solveFIFO2(); //xu ly
	drawTable();  //ve bang
	
	if (!n) fo << "" << n << matTb[0].size() << tr.size();
	fo << "\nTong so loi = " << error << endl; //in ra file
	error = 0; //khoi tao lai so loi
}

int PageReplacement::check(int val) {
	for(int i = 0; i < Opt.size(); ++i)
		if(Opt[i] == val) return 1; //kiem tra trang ton tai hay khong	
	return 0;
}

int PageReplacement::find(int n, int id) {
	int tl = id, vt = -1, t; //tim vi tri xuat hien xa nhat trong tuong lai
	for(int i = 0; i < Opt.size(); ++i)
	{
		t = 0; //bien ktra
		for(int j = id; j < n; ++j) //tim tu vi tri id den cuoi trang
		{
			if(tr[j] == Opt[i]) //neu tim thay
			{
				if(j > tl) //vi tri tim thay lon hon bien tuonglai
				{
					tl = j; //cap nhat bien tuonglai
					vt = i; //cap nhat ket qua
				}
				t = 1; //ktra = 1
				break;
			}
		}
		if(!t) return i; //neu khong tim thay trang hien tai trong tuong lai thi tra ve vi tri trang trong khung
	}

	if (vt > 0) return vt; //bi thay doi gia tri --> tra ve chinh no
	return 0; //khong ton tai tat ca cac trang --> 0
}

int PageReplacement::solveOPT2() {
	int id;
	for(int i = 0; i < n; ++i)
	{
		if(Opt.size() < f) //chua chay den cuoi tien trinh
		{
			if(!check(tr[i])) //trang hien tai chua co trong san trong khung
			{
				Opt.pb(tr[i]); //chen vao vector Opt
				err[i] = i; //luu vi tri loi
				error++; //tang loi len 1
			}	
		}
		else 
		{	
			if(!check(tr[i])) //trang hien tai chua co trong san trong khung
			{
				id = find(n, i + 1); //tim vi tri trang thay the
				backupCot(i, Opt[id], tr[i], f); //chen gia tri vao bang
				Opt[id] = tr[i]; // thay the trang moi 
				err[i] = i; //luu vi tri loi	
				error++; //tang loi len 1
			}
			else backupCot(i, -1, tr[i], f); //chen gia tri trang cua cot dang truoc vao bang
		}	
	}
}

void PageReplacement::solveOPT() {
	fo << "\n\n**Phuong Phap Thay The Trang OPT:**\n\n";
	
	setTb(); //khoi tao bang
	solveOPT2(); //xu ly
	drawTable(); //ve bang
	
	fo << "\nTong so loi = " << error << endl; //in ra file
	error = 0; //khoi tao lai so loi
}

void PageReplacement::solveLRU2() {
	set<int> lru; //luu cac trang dang co
	unordered_map<int, int> id; //luu vi tri cua trang khong can sap xep
	int last; //luu vi tri cua trang xa nhat trong qua khu
	
	for(int i = 0; i < n; i++)
	{
		if(lru.size() < f) //ktra da chay het den cuoi chua
		{
			if(lru.find(tr[i]) == lru.end()) //neu khong tim thay trang
			{
				lru.insert(tr[i]); //chen trang vao set	
				err[i] = i; //danh dau vi tri error
				error++; //tang loi	len 1
			}		
			id[tr[i]] = i; //luu vi tri cua trang i
		}
		else 
		{		
			int last = 9999999;
			if(lru.find(tr[i]) == lru.end()) //neu khong tim thay trang
			{
				err[i] = i; //danh dau error
				error++; //tang loi	len 1
				for(auto it : id)
				{
					//tim vi tri cua trang xa nhat trong qua khu
					if(last > it.second) last = it.second;
				}							
				id.erase(tr[last]); //xoa trang xa nhat ra khoi map	
				lru.erase(tr[last]); //xoa trang xa nhat ra khoi set			
				lru.insert(tr[i]); //chen trang can xet vao set
			}
			backupCot(i, last, tr[i], f); //luu gia tri vao bang
			id[tr[i]] = i; //luu vi tri cua trang dang xet
		}
	}
}

void PageReplacement::solveLRU() {	
	fo << "\n\n**Phuong Phap Thay The Trang LRU:**\n\n";
	
	setTb(); //khoi tao bang
	solveLRU2(); //xu ly
	drawTable(); //ve bang
	
	fo << "\nTong so loi = " << error << endl; //in ra file
}
