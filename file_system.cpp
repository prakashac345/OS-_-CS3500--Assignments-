#include <bits/stdc++.h>
using namespace std;

class inode {
    public:
    string filename;
    int size;
    int type;
    string data_created;
    string data_modified;
    int directblock[3];
    int indexblock;
    inode() {
        filename = "";
        size = 0;
        type = 0;
        data_created = "";
        data_modified = "";
        for(int i = 0; i < 3; i++) {
            directblock[i] = -1;
        }
        indexblock = -1;
    }
    inode(string fname) {
        time_t tim;
        time(&tim);
        filename = fname;
        size = 0;
        type = 0;
        data_created = ctime(&tim);
        data_modified = ctime(&tim);
        for(int i = 0; i < 3; i++) {
            directblock[i] = -1;
        }
        indexblock = -1;
    }
};

vector<pair<bool,string>> block;
vector<int> blockvalue;
map <string, inode> file;
map <string, string> value;
map <string, set<int>> indexblock;
map <string, int> indexblockno;

bool check_block(int k,string filename)
{
    for(int i=0;i<3;i++)
    {
        if(file[filename].directblock[i]== k)
        {
            return false;
        }
    }
    return true;
}

bool fullfree()
{
    int count = 0;
    for(int i = 0; i < block.size(); i++)
    {
        if(block[i].first == false)
        count++;
    }
    if(count == 0) return true;
    return false;
}

bool checkFileName(const char* filename) {
  if (strlen(filename) > 16) {
    return false;
  }
  return true;
}

bool checkDirName(const char* dirname) {
  if (strlen(dirname) > 4) {
    return false;
  }
  return true;
}

int main(int argc, char*argv[])
{
    if(argc <= 2)
    {
        cout<<"Enter Inputs in correct format"<<endl;
        return 0;
    }

    string diskname = argv[1];
    int D = atoi(argv[2]);

    block.resize(D*8);
    blockvalue.resize(D*8);
    int n = D*8;
    for(int i = 0; i < D*8; i++) {
        block[i].first = false;
        block[i].second = "";
    }
    string s;
    int filedisk = (D*8*15)/100;
    bool flag = true;
    while(flag)
    {
        cin>>s;
        if(s == "load")
        {
            string filename;
            cin >> filename;
            if(!checkFileName(filename.c_str())) {
              cout << "File name is too long" << endl;
              continue;
            }
            int fo = 0;
            string dirname = "";
            for(int i=0;i<filename.length();i++)
            {
                if(fo == 0 && filename[i] == '/'){
                    fo = 1;
                    continue;
                }
                else if(fo == 1 && filename[i] == '/')
                {
                    dirname = filename.substr(1,i-1);
                    fo = 2;
                    break;
                }
            }
            if(fo == 2 && !checkDirName(dirname.c_str())) {
              cout << "Dir name is too long" << endl;
              continue;
            }
            if(file.find(filename) != file.end())
            {
                cout << "File already exists" << endl;
                continue;
            }
            else 
            {
                indexblockno[filename] = -1;
                if(file.size() == filedisk)
                {
                    cout << "Disk Directory is full" << endl;
                    continue;
                }
                for(int i=0;i<filedisk;i++)
                {
                    if(block[i].first == false)
                    {
                        block[i].first = true;
                        block[i].second = filename;
                        blockvalue[i] = 128;
                        break;
                    }
                    else continue;
                }
                inode temp(filename);
                file[filename] = temp;
                value[filename] = "";
                //continue;
            }
        }
        else if(s == "delete")
        {
            string filename;
            cin >> filename;
            if(file.find(filename) == file.end())
            {
                cout << "File does not exist" << endl;
                continue;
            }
            else 
            {
                inode temp = file[filename];
                // free all the blocks allocated to file
                for(int i = 0; i < n; i++) {
                    if(block[i].second == filename) {
                        block[i].first = false;
                        block[i].second = "";
                        blockvalue[i] = 0;
                    }
                }
                file.erase(filename);
                value.erase(filename);
                indexblock.erase(filename);
                indexblockno.erase(filename);
            }
        }
        else if(s == "print")
        {
            string filename;
            cin >> filename;
            if(file.find(filename) == file.end())
            {
                cout << "File does not exist" << endl;
                continue;
            }
            else 
            {
                cout << value[filename] << endl;
            }
        }
        else if(s == "append")
        {
            string filename;
            cin >> filename;
            if(file.find(filename) == file.end())
            {
                cout << "File does not exist" << endl;
                continue;
            }
            else 
            {
                string data;
                getline(cin, data);
                data = data.substr(1,data.length());
                time_t tim;
                time(&tim);
                file[filename].data_modified = ctime(&tim);
                vector<int> temp;
                temp.clear();
                int total_data = 0;
                int data_size = data.size();
                int noindexblock = indexblock[filename].size();
                while(data_size > 0)
                {
                    for(int i=filedisk;i<n;i++)
                    {
                        if(fullfree())
                        {
                            cout << "Disk is full" << endl;
                            data_size = 0;
                            break;
                        }
                        if(block[i].first == false)
                        {
                            noindexblock++;
                            if(noindexblock > 32)
                            {
                                cout<<"File Limit exceeded"<<endl;
                                data_size = 0;
                                break;
                            }
                            block[i].first = true;
                            block[i].second = filename;
                            if(data_size > 128)
                            {
                                blockvalue[i] = 128;
                                data_size = data_size - 128;
                                total_data += 128;
                            }
                            else
                            {
                                blockvalue[i] = data_size;
                                total_data += data_size;
                                data_size = 0;
                                break;
                            }
                        }
                        else if (block[i].second == filename)
                        {
                            if(blockvalue[i] == 128) continue;
                            if(blockvalue[i] + data_size > 128)
                            {
                                data_size = data_size - (128 - blockvalue[i]);
                                total_data += (128 - blockvalue[i]);
                                blockvalue[i] = 128;
                            }
                            else
                            {
                                blockvalue[i] += data_size;
                                total_data += data_size;
                                data_size = 0;
                                break;
                            }
                        }
                    }
                }
                for(int i = filedisk ; i < n; i++) {
                    if(check_block(i,filename) && block[i].second == filename && i != indexblockno[filename]) {
                        temp.push_back(i);
                    }
                }
                int cnt = 0;
                for(int i=0;i<3;i++)
                {
                    if(file[filename].directblock[i] != -1) cnt++;
                }
                if(cnt == 3)
                {
                    if(temp.size() != 0 && cnt == 3)
                    {
                        if(file[filename].indexblock == -1)
                        {
                            for(int i=filedisk;i<n;i++)
                            {
                                if(block[i].first == false)
                                {
                                    block[i].first = true;
                                    block[i].second = filename;
                                    file[filename].indexblock = i;
                                    indexblockno[filename] = i;
                                    blockvalue[i] = 128;
                                    break;
                                }
                            }
                        }
                        for(int i=0;i<temp.size();i++)
                        {
                            indexblock[filename].insert(temp[i]);
                        }                        
                    }
                }
                else
                {
                    for(int i=0;i<3;i++)
                    {
                        if(cnt == 3) break;
                        else if(temp.size() == 0) break;
                        else if(file[filename].directblock[i] == -1)
                        {
                            file[filename].directblock[i] = temp[0];
                            temp.erase(temp.begin());
                            cnt++;
                        }
                    }
                    if(temp.size() != 0 && cnt == 3)
                    {
                        if(file[filename].indexblock == -1)
                        {
                            for(int i=filedisk;i<n;i++)
                            {
                                if(block[i].first == false)
                                {
                                    block[i].first = true;
                                    block[i].second = filename;
                                    file[filename].indexblock = i;
                                    indexblockno[filename] = i;
                                    blockvalue[i] = 128;
                                    break;
                                }
                            }
                        }
                        for(int i=0;i<temp.size();i++)
                        {
                            indexblock[filename].insert(temp[i]);
                        }                        
                    }
                }
                file[filename].size += total_data;
                value[filename] += data.substr(0,total_data);
            }
        }
        else if (s == "display")
        {
            if(file.size() == 0)
            {
                cout << "No files" << endl;
                continue;
            }
            for(auto it = file.begin(); it != file.end(); it++)
            {
                //cout << it->first << " " << size << endl;
                cout << it->first << " " << it->second.size << endl;
            }
        }
        else if(s == "freespace")
        {
            int count = 0;
            for(int i = 0; i < block.size(); i++)
            {
                if(block[i].first == false)
                    count++;
            }
            cout << count << endl;
        }
        else if(s == "printinode")
        {
            string filename;
            cin >> filename;
            if(file.find(filename) == file.end())
            {
                cout << "File does not exist" << endl;
                continue;
            }
            else 
            {
                inode temp = file[filename];
                cout << "Filename: " << temp.filename << endl;
                cout << "Size: " << temp.size << endl;
                cout << "Type: " << temp.type << endl;
                cout << "Data Created: " << temp.data_created;
                cout << "Data Modified: " << temp.data_modified;
                cout << "Direct Blocks: ";
                for(int i = 0; i < 3; i++) {
                    cout << temp.directblock[i] << " ";
                }
                cout << endl;
                cout << "Index Block: " << temp.indexblock << endl;
                cout << "Index Block contents: ";
                for(auto i: indexblock[filename]) {
                    cout << i << " ";
                }
                cout << endl;
            }
        }
        else if(s == "exit")
        {
            flag = false;
            cout<<"Exit the System"<<endl;
            break;
        }
        else 
        {
            cout << "Invalid command" << endl;
            flag = false;
        }
    }

    ofstream diskfile(diskname);

    diskfile << "Disk Name: " << diskname << endl;
    diskfile << endl;
    diskfile << "Disk Size: " << n*128 <<" Bytes" << endl;
    diskfile << endl;
    diskfile << "Block Size: " << 128 << endl;
    diskfile << endl;
    diskfile << "Number of Blocks: " << n << endl;
    diskfile << endl;
    diskfile << "Free Blocks: " << endl;
    diskfile << "-------------" << endl;
    int count = 0;
    for(int i = 0; i < block.size(); i++)
    {
        if(block[i].first == false)
        {
            diskfile << i << " ";
            count++;
        }
    }
    diskfile << endl;
    diskfile << endl;
    diskfile << "Number of Free Blocks: " << endl;
    diskfile << "-------------" << endl;   
    diskfile << count<<endl; 
    diskfile << endl;
    diskfile << "Allocated Blocks: " << endl;
    diskfile << "-----------------"<<endl;
    count = 0;
    for(int i = 0; i < block.size(); i++)
    {
        if(block[i].first == true)
        {
            diskfile << i << " ";
            count++;
        }
    }
    diskfile << endl;
    diskfile << endl;
    diskfile << "Number of Allocated Blocks: " << endl;
    diskfile << "-----------------"<<endl;
    diskfile << count << endl;
    diskfile << endl;
    diskfile << "Inode Table: " << endl;
    diskfile << "------------" << endl;
    for(auto it = file.begin(); it != file.end(); it++)
    {
        diskfile << "Filename: " << it->second.filename << endl;
        diskfile << "Size: " << it->second.size << endl;
        diskfile << "Type: " << it->second.type << endl;
        diskfile << "Data Created: " << it->second.data_created;
        diskfile << "Data Modified: " << it->second.data_modified;
        diskfile << "Direct Blocks: ";
        for(int i = 0; i < 3; i++) {
            diskfile << it->second.directblock[i] << " ";
        }
        diskfile << endl;
        diskfile << "Index Block: " << it->second.indexblock << endl;
        diskfile << "Index Block contents: ";
        for(auto i: indexblock[it->first]) {
            diskfile << i << " ";
        }
        diskfile << endl;
        diskfile << endl;
    }
    diskfile << endl;
    diskfile << "Directory Disk Information: " << endl;
    diskfile << "-----------------------------" << endl;
    for(auto it = file.begin(); it != file.end(); it++)
    {
        diskfile << it->first << " " << it->second.size<< endl;
    }
    diskfile << endl;
    diskfile << "File Inode Block Information: " << endl;
    diskfile << "-------------------------------" << endl;
    for(int i=0;i<filedisk;i++)
    {
        if(block[i].first == true)
        {
            diskfile << block[i].second << " " << i << endl;
        }
    }
    diskfile << endl;
    diskfile << "Index Block Information: " << endl;
    diskfile << "-------------------------"<< endl;
    for(int i=0;i<filedisk;i++)
    {
        if(block[i].first == true)
        {
            diskfile << "FileName : " << block[i].second << endl;
            diskfile << "Index Block Number : " << i << endl;
            diskfile << "Index Block Contents : ";
            for(auto it = indexblock[block[i].second].begin(); it != indexblock[block[i].second].end(); it++)
            {
                diskfile << *it << " ";
            }
            diskfile << endl;
            diskfile << endl;
        }
    }
    diskfile << endl;
    diskfile << "Free Data Block Information: " << endl;
    diskfile << "-----------------------------"<< endl;
    count = 0;
    for(int i=filedisk;i<block.size();i++)
    {
        if(block[i].first == false)
        {
            diskfile << i << " ";
            count ++;
        }
    }
    diskfile << endl;
    diskfile << endl;
    diskfile << "Number of Free Data Blocks: " << count << endl;
    diskfile << endl;

    diskfile.close();
    return 0;
}