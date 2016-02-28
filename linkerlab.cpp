//MaryEileen Fagan
//OS Spring 2016
//Lab 1- Linker

#include <iostream>
#include <algorithm>
#include <string>
#include <vector>



using namespace std;

void passTwo(vector<int> rtable, vector<string> stable, vector<int> modadd, vector<string> & NDS, vector<int> & NDR, vector<string> & NUS, vector<int> & NUR, vector<string> & NTS, vector<int> & NTR, vector<int> & dvector, vector<int> & uvector,vector<int> & tvector, vector<string> & defused, vector<int> & defusedmod){
    int q = NTS.size();
    vector<int> eflags;
    vector<int> chains;
    bool arr[q];
    for (int i =0; i<q; i++){
        arr[i] = false;
    }
    vector<string> zerotable, deftable;
    int k=0, j, value, temp, fixer, fixerone=0, rvalue=0;
    for (int i=0; i<modadd.size(); i++){
        for(int j=0; j<tvector[i]; j++){
            if (NTS[k]=="R"){
                NTR[k]=NTR[k] + modadd[i];
            }
            k++;
        }
    }
    
    int p=0, t;
    j=0;
   for (int i=0; i<uvector[p]; i++){
            while (NUS[i]!=stable[j]){
                j++;
                if (j>=stable.size()) break;
            }
       if (j>=stable.size()){
            rvalue = 0;
       }
       else rvalue = rtable[j];
       value = NUR[i]%1000;
            while ((value!=777)&&(value<=tvector[p])){
                temp = NTR[value]%1000;
                if (NTS[value]!="E") eflags.push_back(value+fixer);
                arr[value]=true;
                NTR[value] = NTR[value]/1000*1000+rvalue;
                if (rvalue==0){
                    zerotable.push_back(NTS[value]);
                    deftable.push_back(NUS[i+fixerone]);
                }
                value = temp;
            }
         if ((value>rtable[j]) && (value!=777)) chains.push_back(NTR[value]);
}
    for (int p=1; p<uvector.size();p++){
       fixerone = fixerone + uvector[p-1];
    for (int i=0; i<uvector[p]; i++){
            j=0;
            while (NUS[i+fixerone]!=stable[j]){
                j++;
                if (j>=stable.size()) break;
            }
        if (j>=stable.size()){
            rvalue = 0;
        }
        else rvalue = rtable[j];
            value = NUR[i+fixerone]%1000;
            while ((value!=777) && value<=tvector[p]){
                fixer = modadd[p];
                arr[value+fixer]=true;
                if (NTS[value+fixer]!="E") eflags.push_back(value+fixer);
                temp = NTR[value+fixer]%1000;
                NTR[value+fixer] = NTR[value+fixer]/1000*1000+rvalue;
                if (rvalue==0){
                   zerotable.push_back(NTS[value]);
                    deftable.push_back(NUS[i+fixerone]);
                }
                value = temp;
            }
        if ((value>rtable[j]) && (value!=777)) chains.push_back(NTR[value]);
        }
   }
    
    cout << endl << "Memory Map" << endl;
    for (int i=0; i<NTS.size(); i++){
        cout << i << ": " << " " << NTR[i];
        if ((NTS[i]=="E") && (arr[i]==false)) cout << "  Error: E type address not on use chain; treated as I type.";
        for (int j=0; j<chains.size(); j++){
            if (i==chains[j]) cout << "  Error: Pointer in use chain exceeds module size; chain terminated.";
        }
        for (int j=0; j<zerotable.size(); j++){
            if (NTS[i] == zerotable[j]){
                cout << "     Error: " << deftable[j] << " is not defined; zero used";
                break;
            }
        }
        for (int j=0; j<eflags.size(); j++){
            if (i==eflags[j]) cout << "  Error: " << NTS[i] << " type address on use chain; treated as E type.";
        }
        cout << endl;
    }
    
    for (int i=0; i<defused.size();i++){
        cout << endl << "Warning: " << defused[i] << " was used in module " << defusedmod[i] << " but never used.";
    }
    
       
        
}


void passOne(vector<string> & NDS, vector<string> & NUS, vector<string> & NTS, vector<int> & NDR, vector<int> & NUR, vector<int> & NTR, vector<int> & dvector, vector<int> & uvector,
    vector<int> & tvector){
    vector<int> modadd(1, 0);
    bool definedused = false, repeatsyesno=false;
	vector<int> rtable;
	vector<string> stable;
    vector<string> defexceeded;
    vector<int> defnumexceeded;
    vector<int> exceededmodadd;
    vector<int> repeats;
    vector<int> erased;
    vector<string> defused;
    vector<int> defusedmod;
    int d, p;
    
    for (int i=0; i<NDS.size(); i++){
        stable.push_back(NDS[i]);
    }

    for (int i=0; i<stable.size(); i++){
        for (int j=stable.size()-1; j>i; j--){
            if (stable[i]==stable[j]){
                repeats.push_back(i);
                erased.push_back(j);
                stable.erase (stable.begin()+j);
            }
        }
    }
    
    
    for (int i=1; i<dvector.size(); i++){
        modadd.push_back(modadd[i-1] + tvector[i-1]);
   
    }
    
    int k=0;
    for (int i=0; i<dvector.size(); i++){
        if (dvector[i]>0){
            for (int j=0; j<dvector[i]; j++){
                if ( ((NDR[k]+1)>tvector[i])){
                    rtable.push_back(modadd[i]);
                    defexceeded.push_back(NDS[k]);
                    defnumexceeded.push_back(NDR[k]);
                    exceededmodadd.push_back(i);
                }
                else{
                    rtable.push_back(NDR[k] + modadd[i]);
                }
                k++;
            }
        }	
        
    }
    for (int i=0; i<erased.size(); i++){
        rtable.erase(rtable.begin()+erased[i]);
    }

    cout << "Symbol Table"<< endl;
    for (int i=0; i<stable.size();i++){
        cout << stable[i] << "=" << rtable[i];
        for (int j=0; j<repeats.size(); j++){
          if (i==repeats[j]){
            repeatsyesno=true;
            cout << "  Error: This variable is multiply defined; first value used.";
            }
        }
        if (repeatsyesno==false){
            for (int j=0; j<defexceeded.size(); j++){
                if ((defexceeded[j]==stable[i])){
                    cout << " Error: The value of " << stable[i] << "  is outside module " << exceededmodadd[j] <<  "; zero (relative) used.";
                }
            }
        }

        cout << endl;

    }
    d=0;
    p=0;
    for (int i=0; i<NDS.size(); i++){
        while (d>=dvector[p]){
            p++;
            d=0;
        }
        definedused=false;
        for (int j=0; j<NUS.size(); j++){
            if (NDS[i]==NUS[j]){
                definedused=true;
                }
        }
        if (definedused==false){
            defused.push_back(NDS[i]);
            defusedmod.push_back(p);
        }
           d++;

    }
    
    
	
    passTwo(rtable, stable, modadd, NDS, NDR, NUS, NUR,  NTS, NTR, dvector, uvector, tvector, defused, defusedmod);
}


void scanin(){
	int d, u, t;
	vector<string> NDS;
	vector <string> NUS;
	vector <string> NTS;
	vector<int> NDR;
	vector <int> NUR;
	vector <int> NTR;
	vector <int> dvector; 
	vector <int> tvector;
	vector <int> uvector;
	int intinput;
	string strinput;
	int i, pass=0;
    

while (!cin.eof()){
	pass++;
	cin>>d;
	dvector.push_back(d);
    i=0;
    while (i<d){
        cin>>strinput;
        NDS.push_back(strinput);
        cin>>intinput;
        NDR.push_back(intinput);
        i++;
    }

    
    cin>>u;
	uvector.push_back(u);
    i=0;
    while (i<u){
        cin>>strinput;
        NUS.push_back(strinput);
        cin>>intinput;
        NUR.push_back(intinput);
        i++;
    }
    
    
    cin>>t;
	tvector.push_back(t);
    i=0;
    while (i<t){
        cin>>strinput;
        NTS.push_back(strinput);
        cin>>intinput;
        NTR.push_back(intinput);
        i++;
    }
    }
 

    
        
        
        
        
passOne(NDS, NUS, NTS, NDR, NUR, NTR, dvector, uvector, tvector);
}


int main(){
    scanin();
    return 0;
}
