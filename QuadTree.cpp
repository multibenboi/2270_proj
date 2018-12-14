#include <string>
#include <iostream>
#include <vector>
#include <fstream>
#include <cmath>

using namespace std;

struct quad_node{
    string node_color;
    vector<string> child_colors;
    quad_node* NW_node;
    quad_node* NE_node;
    quad_node* SW_node;
    quad_node* SE_node;
    
};

//takes pbm ascii file and creates vector of strings
vector<string> img_to_arr(string file_name){
    
    ifstream convert_file(file_name);
    string work_str;
    vector<string> file_vec;
    
    while(convert_file >> work_str){
        file_vec.push_back(work_str);
        
    }
    convert_file.close();
    return file_vec;
}



//convert bit string into quad node
quad_node* construct_quad_node(vector<string> in_arr){
    unsigned int n2=in_arr.size();
    string zero="0";
    string black="B";
    string white="W";
    string gray="G";
    
    //create node from array
    if(n2==4){
        
        quad_node* ret_node=new quad_node;
        string check=in_arr.at(0);
        
        for(int l=0;l<4;l++){
            ret_node->child_colors.push_back(in_arr.at(l));
            if(in_arr.at(l)!=check){
                ret_node->node_color=gray;
            }
        }
        if(ret_node->node_color!=gray){
            if(in_arr.at(0)==zero){
                ret_node->node_color=white;
            }
            else{
                ret_node->node_color=black;
            }
        }
        return ret_node;
    }
    
    unsigned int n=sqrt(n2);
    
    //create string vectors for each of the  cardinal directions
    vector<string> NWvec;
    vector<string> NEvec;
    vector<string> SWvec;
    vector<string> SEvec;
    for(unsigned int i=0;i<n2;i=i+n){
        for (unsigned int k=0;k<n;k++){
            if((i+k)%n<(n/2) && i<n2/2){
                NWvec.push_back(in_arr.at(i+k));
            }
            else if((i+k)%n>=(n/2) && i<n2/2){
                NEvec.push_back(in_arr.at(i+k));
            }
            else if((i+k)%n<(n/2) && i>=n2/2){
                SWvec.push_back(in_arr.at(i+k));
            }
            else{
                SEvec.push_back(in_arr.at(i+k));
            }
        }
            
    }
    //create node from each vector recursive call
    quad_node* NW_node=construct_quad_node(NWvec);
    quad_node* NE_node=construct_quad_node(NEvec);
    quad_node* SW_node=construct_quad_node(SWvec);
    quad_node* SE_node=construct_quad_node(SEvec);
    
    //initialize return node
    quad_node* ret_node=new quad_node;
    
    //set children
    ret_node->NW_node=NW_node;
    ret_node->NE_node=NE_node;
    ret_node->SW_node=SW_node;
    ret_node->SE_node=SE_node;
    
    //find color of node based on children
    string check2=NW_node->node_color;
    if(check2==gray){
        ret_node->node_color=gray;
    }
    else if(check2!=NE_node->node_color){
        ret_node->node_color=gray;
    }
    else if(check2!=SW_node->node_color){
        ret_node->node_color=gray;
    }
    else if(check2!=SE_node->node_color){
        ret_node->node_color=gray;
    }
    else{
        ret_node->node_color=check2;
    }
    
    //set child colors;
    ret_node->child_colors.push_back(NW_node->node_color);
    ret_node->child_colors.push_back(NE_node->node_color);
    ret_node->child_colors.push_back(SW_node->node_color);
    ret_node->child_colors.push_back(SE_node->node_color);
    
    return ret_node;
}



// take two trees, returns the union of the two trees
quad_node* compare_imgs(quad_node* img_tree, quad_node* img2_tree){
    
    string zero="0";
    string one="1";
    string black="B";
    string white="W";
    string gray="G";
    
    //compare color of nodes to produce
    //if one is black, copy black tree
    if(img_tree->node_color==black){
        return img_tree;
    }
    else if(img2_tree->node_color==black){
        return img2_tree;
    }
    //if one is white copy other tree
    else if(img_tree->node_color==white){
        return img2_tree;
    }
    else if(img2_tree->node_color==white){
        return img_tree;
    }
    
    //if both gray, create new node, recurse and set as children
    else{
        //if leaf node, construct new node, compare, fill in, return
        if(img_tree->NW_node==NULL){
            quad_node* ret=new quad_node;
            //NW
            if(img_tree->child_colors.at(0)==one ||img2_tree->child_colors.at(0)==one){
                ret->child_colors.push_back(one);
            }
            else{
                ret->child_colors.push_back(zero);
            }
            //NE
            if(img_tree->child_colors.at(1)==one ||img2_tree->child_colors.at(1)==one){
                ret->child_colors.push_back(one);
            }
            else{
                ret->child_colors.push_back(zero);
            }
            //SW
            if(img_tree->child_colors.at(2)==one ||img2_tree->child_colors.at(2)==one){
                ret->child_colors.push_back(one);
            }
            else{
                ret->child_colors.push_back(zero);
            }
            //SE
            if(img_tree->child_colors.at(3)==one ||img2_tree->child_colors.at(3)==one){
                ret->child_colors.push_back(one);
            }
            else{
                ret->child_colors.push_back(zero);
            }
            //check color of children to make color of node
            string check_c=ret->child_colors.at(0);
            
            ret->node_color=zero;
            for(int i=1;i<4;i++){
                if(ret->child_colors.at(i)!=check_c){
                    ret->node_color=gray;
                }
            }
            if(ret->node_color!=gray){
                if(check_c==zero){
                    ret->node_color=white;
                }
                else{
                    ret->node_color=black;
                }
            }
            
            return ret;
            
        }
        
        //gray nodes non-leaf
        else{
            quad_node* ret=new quad_node;
            quad_node* NW_node=compare_imgs(img_tree->NW_node,img2_tree->NW_node);
            quad_node* NE_node=compare_imgs(img_tree->NE_node,img2_tree->NE_node);
            quad_node* SW_node=compare_imgs(img_tree->SW_node,img2_tree->SW_node);
            quad_node* SE_node=compare_imgs(img_tree->SE_node,img2_tree->SE_node);
            
            ret->NW_node=NW_node;
            ret->NE_node=NE_node;
            ret->SW_node=SW_node;
            ret->SE_node=SE_node;
            
            //set NW node color as check color, add that colors to child_colors
            string check=ret->NW_node->node_color;
            ret->child_colors.push_back(NW_node->node_color);
            ret->child_colors.push_back(NE_node->node_color);
            ret->child_colors.push_back(SW_node->node_color);
            ret->child_colors.push_back(SE_node->node_color);
            
            //compare child colors
            if(ret->child_colors.at(1)!=check){
                ret->node_color=gray;
            }
            else if(ret->child_colors.at(2)!=check){
                ret->node_color=gray;
            }
            else if(ret->child_colors.at(3)!=check){
                ret->node_color=gray;
            }
            else {
                ret->node_color=check;
            }
            
            return ret;
        }
        
        
        
    }
    
    
}


//tree_to_vec unable to complete
//vector<string>  tree_to_vec(quad_tree* in){
    //counter=1;
    //quad_tree* next=in->NW_node;
    
    //make array of necessary size,
    //in order walk, fill in
    //return bits
//}




//takes array to convert to pbm file and desired file name and creates file
void arr_to_img(vector<string> out_arr,string file_name){
    
    ofstream outfile(file_name);
    
    for(unsigned int i=0;i<out_arr.size();i++){
        outfile<<out_arr.at(i);
        outfile<<" ";
        if((i-6)%40==0){
            outfile<<endl;
        }
    }
    
}


//add check for sizing?
int main(){
    //convert first file to vector of strings
    string file1;
    cout<< "Please enter name of the first file with pbm extenison."<<endl;
    cin>>file1;
    vector<string> file1_arr=img_to_arr(file1);
    
    //convert second file to vector of strings
    string file2;
    cout<< "Please enter name of the second file with pbm extenison."<<endl;
    cin>>file2;
    vector<string> file2_arr=img_to_arr(file2);
    
    //REQUIRE(file1_arr.size()==file2_arr.size())//vectors must be same size so images can be compared
    
    //create output vector with same size as file1
    vector<string> out_arr=file1_arr;
    
    //erase header of file to leave only 0s and 1s
    for(int k=0;k<7;k++){
        file1_arr.erase(file1_arr.begin());
        file2_arr.erase(file2_arr.begin());
        
    }
    
    //take files and create quadtrees
    quad_node* file1_node=construct_quad_node(file1_arr);
    quad_node* file2_node=construct_quad_node(file2_arr);
    
    //create union of two quad nodes
    quad_node* combo=compare_imgs(file1_node,file2_node);
    
    
    
    //get desired name of output file
    string outfile_name;
    cout<< "Please enter name of the output file with pbm extenison."<<endl;
    cin>>outfile_name;
    
    arr_to_img(out_arr,outfile_name);
    
    delete file1_node;
    delete file2_node;
    delete combo;
    
    
}


