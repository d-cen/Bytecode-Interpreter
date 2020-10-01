#include <iostream>
#include <fstream>
#include <istream>
#include <vector>
#include <stdio.h>
#include <cstdint>
#include <list>

using namespace std;

class Memory{
public:
    uint8_t val;
    static int pc;
    static int sp;
    static int fpsp;
    static list<int> rstack;
    static list<int> fpstack;
    static vector<uint8_t> map;

    Memory(uint8_t v){
        val = v;
    }
    ~Memory(){ }

    void execute(uint8_t cmd){
        switch(cmd){
            case 36:{ //jmp
                pc = rstack.back();
                rstack.pop_back();
                sp -= 2;
                break;
            }
            case 40: { //jmpc
                int addr = rstack.back();
                rstack.pop_back();
                int val = rstack.back();
                rstack.pop_back();
                if (val == 1) {
                	pc = addr;
                }
                else {
                	pc++;
                }
                sp -= 2;
                break;
            }
            case 44:{ //call
                fpsp++;
                fpstack.push_back(sp-rstack.back()-1);
                rstack.pop_back();
                sp--;
                pc = rstack.back();
                rstack.pop_back();
                sp--;
                break;
            }
            case 48:{ //ret
                sp = fpstack.back();
                fpstack.pop_back();
                fpsp--;
                pc = rstack.back();
                rstack.pop_back();
                sp--;
                break;
            }
            case 68:{ //pushc
                sp++;
                rstack.push_back(map[++pc]);
                pc++;
                break;
            }
            case 69:{ //pushs
                sp++;
                uint8_t b1 = map[pc+1];
                uint8_t b2 = map[pc+2];
                int num = int(b2) << 8 | b1;
                rstack.push_back(num);
                pc += 3;
                break;
            }
            case 70:{ //pushi
                sp++;
                uint8_t b1 = map[pc+1];
                uint8_t b2 = map[pc+2];
                uint8_t b3 = map[pc+3];
                uint8_t b4 = map[pc+4];
                int num = int(b4) << 24 | int(b3) << 16 | int(b2) << 8 | int(b1);
                rstack.push_back(num);
                pc += 5;
                break;
            }
            case 71:{ //pushf
                sp++;
                uint8_t b1 = map[pc+1];
                uint8_t b2 = map[pc+2];
                uint8_t b3 = map[pc+3];
                uint8_t b4 = map[pc+4];
                int num = int(b4) << 24 | int(b3) << 16 | int(b2) << 8 | int(b1);
                rstack.push_back(num);
                pc += 5;
                break;
            }
            case 72:{ //pushvc
                int i = fpstack.back() + rstack.back() + 1;
                auto num = next(rstack.begin(), i);
                rstack.pop_back();
                rstack.push_back(*num);
                pc++;
                break;
            }
            case 73:{ //pushvs
                int i = fpstack.back() + rstack.back() + 1;
                auto num = next(rstack.begin(), i);
                rstack.pop_back();
                rstack.push_back(*num);
                pc++;
                break;
            }
            case 74:{ //pushvi
            	int i = fpstack.back() + rstack.back() + 1;
            	auto num = next(rstack.begin(), i);
            	rstack.pop_back();
            	rstack.push_back(*num);
            	pc++;
            	break;
            }
            case 75:{ //pushvf
                int i = fpstack.back() + rstack.back() + 1;
                auto num = next(rstack.begin(), i);
                rstack.pop_back();
                rstack.push_back(*num);
                pc++;
                break;
            }
            case 76:{ //popm
            	int num = rstack.back() + 1;
            	sp -= num;
            	while (num!=0) {
            		rstack.pop_back();
            		num--;
            	}
            	pc++;
            	break;
            }
            case 77:{ //popa
            	int val1 = sp;
            	int val2 = fpstack.back();
                while(val2 < val1){
                    rstack.pop_back();
                    sp--;
                    val2++;
                }
                pc++;
                break;
            }
            case 80: { //popv
            	rstack.pop_back();
            	int num = rstack.back();
            	rstack.pop_back();
            	rstack.pop_back();
            	rstack.push_back(num);
            	sp -= 2;
            	pc++;
            	break;
            }
            case 84: { //peekc
            	int val = fpstack.back() + rstack.back() + 1;
            	rstack.pop_back();
            	sp--;
                int i = fpstack.back() + rstack.back() + 1;
                int popct = 0;
                while(i != sp) {
                    rstack.pop_back();
                    i++;
                    popct++;
                }
                rstack.pop_back();
                rstack.push_back(val);
                sp -= popct;
                pc++;
                break;
            }
            case 85: { //peeks
            	int val = fpstack.back() + rstack.back() + 1;
            	rstack.pop_back();
            	sp--;
            	int i = fpstack.back() + rstack.back() + 1;
                int popct = 0;
                while(i != sp) {
                    rstack.pop_back();
                    i++;
                    popct++;
                }
                rstack.pop_back();
              	rstack.push_back(val);
                sp -= popct;
                pc++;
                break;
            }
            case 86: { //peeki
            	auto val = next(rstack.begin(), (fpstack.back() + rstack.back() + 1));
            	rstack.pop_back();
            	sp--;
            	int i = fpstack.back() + rstack.back() + 1;
            	int popct = 0;
            	while(i != sp) {
            		rstack.pop_back();
            		i++;
            		popct++;
            	}
            	rstack.pop_back();
            	rstack.push_back(*val);
            	sp -= popct;
            	pc++;
            	break;
            }
            case 87: { //peekf
            	int val = fpstack.back() + rstack.back() + 1;
                rstack.pop_back();
            	sp--;
            	int i = fpstack.back() + rstack.back() + 1;
                int popct = 0;
                while(i != sp) {
                    rstack.pop_back();
                    i++;
                    popct++;
                }
                rstack.pop_back();
                rstack.push_back(val);
                sp -= popct;
                pc++;
                break;
            }
            case 88:{ //poke
                list<int>::iterator it = rstack.begin(); // create iterator instance
                int didx = fpstack.back() + rstack.back() + 1; // index of rstack on lhs
                rstack.pop_back();
                int sidx = fpstack.back() + rstack.back() + 1; // index of rstack on rhs
                advance(it, sidx); // move iterator to position
                int num = *it; // assign num = rstack[sidx]
                rstack.pop_back();
                sp -= 2;
                it = rstack.begin(); // reset iterator position
                advance(it, didx); // move iteratator to position
                rstack.insert(it, num); // insert new element at rstack[didx], shifting entire list back
                rstack.erase(++it); // erase original element at rstack[didx]
                pc++;
            	break;
            }
            case 89:{ //poke
                list<int>::iterator it = rstack.begin(); // create iterator instance
                int didx = fpstack.back() + rstack.back() + 1; // index of rstack on lhs
                rstack.pop_back();
                int sidx = fpstack.back() + rstack.back() + 1; // index of rstack on rhs
                advance(it, sidx); // move iterator to position
                int num = *it; // assign num = rstack[sidx]
                rstack.pop_back();
                sp -= 2;
                it = rstack.begin(); // reset iterator position
                advance(it, didx); // move iteratator to position
                rstack.insert(it, num); // insert new element at rstack[didx], shifting entire list back
                rstack.erase(++it); // erase original element at rstack[didx]
                pc++;
                break;
            }
            case 90:{ //poke
                list<int>::iterator it = rstack.begin(); // create iterator instance
                int didx = fpstack.back() + rstack.back() + 1; // index of rstack on lhs
                rstack.pop_back();
                int sidx = fpstack.back() + rstack.back() + 1; // index of rstack on rhs
                advance(it, sidx); // move iterator to position
                int num = *it; // assign num = rstack[sidx]
                rstack.pop_back();
                sp -= 2;
                it = rstack.begin(); // reset iterator position
                advance(it, didx); // move iteratator to position
                rstack.insert(it, num); // insert new element at rstack[didx], shifting entire list back
                rstack.erase(++it); // erase original element at rstack[didx]
                pc++;
                break;
            }
            case 91:{ //poke
                list<int>::iterator it = rstack.begin(); // create iterator instance
                int didx = fpstack.back() + rstack.back() + 1; // index of rstack on lhs
                rstack.pop_back();
                int sidx = fpstack.back() + rstack.back() + 1; // index of rstack on rhs
                advance(it, sidx); // move iterator to position
                int num = *it; // assign num = rstack[sidx]
                rstack.pop_back();
                sp -= 2;
                it = rstack.begin(); // reset iterator position
                advance(it, didx); // move iteratator to position
                rstack.insert(it, num); // insert new element at rstack[didx], shifting entire list back
                rstack.erase(++it); // erase original element at rstack[didx]
                pc++;
                break;
            }
            case 94:{ //swp
            	int temp1 = rstack.back();
            	rstack.pop_back();
            	int temp2 = rstack.back();
            	rstack.pop_back();
            	rstack.push_back(temp1);
            	rstack.push_back(temp2);
            	pc++;
            	break;
            }
            case 100:{ //add
            	int opd2 = rstack.back();
            	rstack.pop_back();
            	int opd1 = rstack.back();
            	rstack.pop_back();
            	rstack.push_back(opd1 + opd2);
            	sp--;
            	pc++;
            	break;
            }
            case 104:{ //sub
                int opd2 = rstack.back();
                rstack.pop_back();
                int opd1 = rstack.back();
                rstack.pop_back();
                rstack.push_back(opd1 - opd2);
                sp--;
                pc++;
                break;
            }
            case 108:{ //mul
                int opd2 = rstack.back();
                rstack.pop_back();
                int opd1 = rstack.back();
                rstack.pop_back();
                rstack.push_back(opd1 * opd2);
                sp--;
                pc++;
                break;
            }
            case 112:{ //div
                int opd2 = rstack.back();
                rstack.pop_back();
                int opd1 = rstack.back();
                rstack.pop_back();
                rstack.push_back(opd1 / opd2);
                sp--;
                pc++;
                break;
            }
            case 132:{ //cmpe
                int opd1 = rstack.back();
                rstack.pop_back();
                int opd2 = rstack.back();
                rstack.pop_back();
                int num = opd1 == opd2 ? 1 : 0;
                rstack.push_back(num);
                sp--;
                pc++;
                break;
            }
            case 136:{ //cmplt
            	int opd2 = rstack.back();
            	rstack.pop_back();
            	int opd1 = rstack.back();
            	rstack.pop_back();
            	rstack.push_back(opd1 < opd2 ? 1 : 0);
            	sp--;
            	pc++;
            	break;
            }
            case 140:{ //cmpgt
                int opd2 = rstack.back();
                rstack.pop_back();
                int opd1 = rstack.back();
                rstack.pop_back();
                rstack.push_back(opd1 > opd2 ? 1 : 0);
                sp--;
                pc++;
                break;
            }
            case 144:{ //printc
                cout << rstack.back() << endl;
                rstack.pop_back();
                sp--;
                pc++;
                break;
            }
            case 145:{ //prints
                cout << rstack.back() << endl;
                rstack.pop_back();
                sp--;
                pc++;
                break;
            }
            case 146:{ //printi
                std::cout << rstack.back() << endl;
                rstack.pop_back();
                sp--;
                pc++;
                break;
            }
            case 147:{ //printf
                cout << rstack.back() << endl;
                rstack.pop_back();
                sp--;
                pc++;
                break;
            }
            case 0:{ //halt
            	cout << "\nCompile values:\n";
            	cout << "PC: " << pc << "\n";
            	cout << "SP: " << sp << "\n";
            	if (rstack.empty()) {
            		cout << "RSTACK: empty\n";
            	}
            	else {
            		cout << "RSTACK:";
            		for (auto item : rstack) {
            			cout << " " << item;
            		}
            		cout << "\n";
            	}
            	cout << "FPSP: " << fpsp << "\n";
            	if (fpstack.empty()) {
            	    cout << "FPSTACK: empty\n";
            	}
            	else {
            	    cout << "FPSTACK:";
            	    for (auto item : fpstack) {
            	        cout << " " << item;
            	    }
            	    cout << "\n" << endl;
            	}
            	exit(1);
            }
        }
    }
};

int Memory::pc = 0;
int Memory::sp = -1;
int Memory::fpsp = -1;
list<int> Memory::rstack;
list<int> Memory::fpstack;
vector<uint8_t> Memory::map;

void print_vec(int len, vector<uint8_t> map){
    for(int i = 0; i < len; i++){
        cout << int(map[i]) << " ";
    }
    cout << endl;
}

int main(int argc, const char * argv[]) {
    char b;
    vector<uint8_t> mmap;
    vector<Memory> vamp;
    std::ifstream bt(argv[1]);
    if(!bt){
        cerr << "Error opening file.";
        exit(1);
    }
    bt.seekg(0, ios::end);
    int len = (int)bt.tellg();
    bt.seekg(0, ios::beg);
    while(len>bt.tellg()){
        bt.get(b);
        mmap.push_back(b);
        vamp.push_back(Memory(b));
    }
    Memory::map = mmap;
    do{
        Memory m = vamp[Memory::pc];/*
        cout << "State Before Executing PC " << Memory::pc << "(Instr: " << int(vamp[Memory::pc].val) << ")" << endl;
        cout << endl;
        cout << "PC: " << Memory::pc << endl;
        cout << "MEM At and Near PC:";
        for(int i = 0; i < 10; i++){
            cout << " " << int(mmap[i+Memory::pc]);
        }
        cout << endl;
        cout << "SP: " << Memory::sp << endl;
        cout << "RSTACK:";
        for (auto m : Memory::rstack)
            cout << " " << m;
        cout << endl;
        cout << "FPSP: " << Memory::fpsp << endl;
        cout << "FPSTACK:";
        for (auto m : Memory::fpstack)
            cout << " " << m;
        cout << endl;
        cout << endl;*/
        m.execute(m.val);/*
        cout << "State After Executing" << endl;
        cout << endl;
        cout << "PC: " << Memory::pc << endl;
        cout << "MEM At and Near PC:";
        for(int i = 0; i < 10; i++){
            cout << " " << int(mmap[i+Memory::pc]);
        }
        cout << endl;
        cout << "SP: " << Memory::sp << endl;
        cout << "RSTACK:";
        for (auto m : Memory::rstack)
            cout << " " << m;
        cout << endl;
        cout << "FPSP: " << Memory::fpsp << endl;
        cout << "FPSTACK:";
        for (auto m : Memory::fpstack)
            cout << " " << m;
        cout << endl << endl;
        cout << "-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-" << endl;*/
    }while(int(vamp[Memory::pc].val)!=-1);
    return 0;
}
