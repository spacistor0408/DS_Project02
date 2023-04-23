
// 10724243 •€≈VÊX

#include <iostream>
#include <vector>
#include <cstdlib>
#include <fstream>
#include <string>
#include <iomanip>
#include <time.h>
#include <math.h>
#include <stack>

using namespace std;

#define PTR_NUM 3
#define KEY_NUM PTR_NUM-1

typedef struct slot {
    vector<int> rSet ; // a set of record id with the same key
    string key ; // record school name
} slotType ;

typedef struct nT { // 2-3 tree node
    slotType data[KEY_NUM] ; // 0 key01, 1 key02
    struct nT *link[PTR_NUM] ; // 0 leftmost, 1 middle, 2 rightmost
    struct nT *parent ; // point to the parent node
} nodeType ;

typedef struct eT {
    slotType slot ;
    nodeType *link ;
} extraType ;

typedef nodeType* nodePtr ;

class DM { // Data manage and In/Output

    vector<vector<string>> orinList;
    string fname ;

    void CombName( string name ) {

        fname = name + fname.substr(5,3) ;
        fname = fname + ".txt" ;

    } // CombName()

    void Checkformat( string name ) {
        // If input a code
        if ( fname.at(0) == '2' && fname.size() == 3 ) {
            fname = name + fname + ".txt" ;
        } // if

    } // Checkformat()

    void ReadFileName() {
        cin >> fname ;
    } // ReadFileName()

public:

    string GetFileName() {
        return fname ;
    } // GetFileName()

    bool ReadFile( string fileType ) {

        orinList.clear() ;

        fstream file ;
        char buffer ;
        string data ;
        vector<string> raw ;


        /*---------- deal with File ----------*/
        do {
       		cout << "Input 201, 202, ...[0]Quit): " ;
      		fname.clear() ;
     		ReadFileName() ;

        	if ( fname == "0" ) {
        		return false ;
			} // Quit to open file

            Checkformat( fileType ) ;

			cout << endl ;

        	file.open( fname.c_str(), ios::in ) ; // open file

        	if ( !file ){
            	cout << "Error:Open file fail" << endl << endl ;
			} // if Error

		} while ( !file ) ;

		/*---------- deal with data ----------*/

        while ( file.get(buffer) ) {

            if ( buffer != '\n' ) {

                if ( buffer == '\t' ) {
                    raw.push_back(data) ; // add new data on list
                    data.clear() ; // clear data
                }else{
                    data = data + buffer ; // complete the string
                } // else

            } else {
                raw.push_back(data) ; // add new data on list
                data.clear() ;
                orinList.push_back(raw) ;
                raw.clear() ;
            } // else

        } // end while

        if ( buffer != '\n' && !raw.empty() ) {
            raw.push_back(data) ; // add new data on list
            data.clear() ;
            orinList.push_back(raw) ;
            raw.clear() ;
        } // end if



        if ( orinList.at(orinList.size()-1).at(0).compare("") == 0 && orinList.size() <= 5 ) {
            orinList.pop_back() ;
        } // if only have a data but it display 2 and the second is empty

        file.close() ;

        return true ;

    } // ReadFile()

    bool PrintAll( ) {
        if ( orinList.empty() ) {
            return false ;
        }

        for ( int i = 0 ; i < orinList.size() ; i++ ) {
        	  cout << "[" << i+1 << "] " ;
            for ( int j = 0 ; j < orinList.at(i).size() ; j++ ) {
                cout << orinList.at(i).at(j) << '\t' ;
            }
            cout << endl ;
        }
        return true ;
    } // PrintAll()

    void Write( string name ) {

        fstream file ;

        CombName( name ) ; // change fname to "ouput"

        file.open( fname.c_str(), ios::out ) ;

        for ( int i = 0 ; i < orinList.size() ; i++ ) {
            for ( int position = 0 ; position < orinList.at(i).size() ; position++ ) {
              file << orinList.at(i).at(position) << '\t' ;
            }
            file << '\n' ;
        }

        file.close();
    } // Write()

    void RemoveTitle( ) {
        orinList.erase(orinList.begin(), orinList.begin() + 3 ) ;
    } // RemoveTitle()

    string GetData( int pos, int num ) {
        return orinList.at(pos).at(num) ;
    } // GetData

    int GetSize( ) {
        return orinList.size() ;
    } // GetSize()

}; // Class FileIO

class TwoThreeTree {

    nodeType *root ;

    nodeType *CreatNode( nodeType *left, nodeType *right, nodeType *pNode, slotType newSlot ) {

        nodeType *newNode = NULL ;

        newNode = new nodeType ;
        newNode->data[0].rSet = newSlot.rSet ;
        newNode->data[1].rSet.clear() ;
        newNode->data[0].key = newSlot.key ;
        newNode->data[1].key = "" ;
        newNode->parent = pNode ;
        newNode->link[0] = left ;
        newNode->link[1] = right ;
        newNode->link[2] = NULL ;

        return newNode;

    } // CreatNode()

    void Compare( slotType &s1, slotType &s2 ) {

        if ( s1.key > s2.key ) {
            swap( s1, s2 ) ;
        } // if

    } // Compare()

    void insertLeaf( nodeType *cur, slotType newSlot ) {

        // have only one key

        if ( !cur->data[0].rSet.empty() && cur->data[1].rSet.empty() ) {
            cur->data[1].key = newSlot.key ;
            cur->data[1].rSet = newSlot.rSet ;

            if ( cur->data[0].key.compare( cur->data[1].key ) )
                swap( cur->data[0], cur->data[1] ) ;

            cout << "[func]" << cur->data[1].rSet.at(0) << endl ; // debug line
        }

    } // insertLeaf()

    void splitLeaf( nodeType *cur, slotType newSlot, extraType extraNode ) {

        slotType buf[PTR_NUM] ;

        buf[0] = cur->data[0] ;
        buf[1] = cur->data[1] ;
        buf[2] = newSlot ;

        // sort the buf
        if ( buf[2].key.compare( buf[1].key ) < 0 )
            swap( buf[2], buf[1] ) ;
        if ( buf[1].key.compare( buf[0].key ) < 0 )
            swap( buf[1], buf[0] ) ;

        cur->data[0] = buf[0] ; // only record leftmost
        cur->data[1].rSet.clear() ;

        extraNode.link = CreatNode(NULL, NULL, cur->parent, buf[2] ) ;
        extraNode.slot = buf[1] ;

    } // splitLeaf()

    nodeType *CreatRoot( nodeType *left, nodeType *right, slotType oneSlot ) {

        nodeType *newRoot = CreatNode( left, right, NULL, oneSlot ) ;
        left->parent = newRoot ;
        right->parent = newRoot ;

        return newRoot ;
    } // CreatRoot()

    void insertNonleaf( extraType newNode, nodeType *cur ) {
        cur->data[1] = newNode.slot ;
        cur->link[2] = newNode.link ;

        if ( cur->data[0].key.compare( newNode.slot.key ) > 0 ) { // if key01 > new key
            swap( cur->data[0], cur->data[1] ) ;
            swap( cur->link[1], cur->link[2] ) ;
        } // if

    } // insertNonleaf()

    void splitNonleaf( nodeType *cur, extraType newNode ) {
        slotType buf[PTR_NUM] ;
        nodeType *childBuf[PTR_NUM+1] ; // a buffer to keep pointers of children

        buf[0] = cur->data[0] ;
        buf[1] = cur->data[1] ;
        buf[2] = newNode.slot ;

        // sort the buf
        if ( buf[2].key.compare( buf[1].key ) < 0 )
            swap( buf[2], buf[1] ) ;
        if ( buf[1].key.compare( buf[0].key ) < 0 )
            swap( buf[1], buf[0] ) ;

        // sort the childBuf
        childBuf[0] = cur->link[0] ;
        childBuf[1] = cur->link[1] ;
        childBuf[2] = cur->link[2] ;
        childBuf[3] = newNode.link ;

        if ( childBuf[3]->data[0].key.compare( childBuf[2]->data[0].key ) < 0 )
            swap( childBuf[3], childBuf[2] ) ;
        if ( childBuf[2]->data[0].key.compare( childBuf[1]->data[0].key ) < 0 )
            swap( childBuf[2], childBuf[1] ) ;
        if ( childBuf[1]->data[0].key.compare( childBuf[0]->data[0].key ) < 0 )
            swap( childBuf[1], childBuf[0] ) ;

        // ------------------------------------------------------------------

        cur->data[0] = buf[0] ;
        cur->link[0] = childBuf[0] ;
        cur->link[1] = childBuf[1] ;

        nodeType *sibling = CreatNode( childBuf[2], childBuf[3], cur->parent, buf[2] ) ;

        newNode.slot = buf[1] ;
        newNode.link = sibling ;

    } // splitNonleaf()

    void search( string name, stack<nodePtr> &path ) {

        nodeType *temp = root ;
        temp->parent = NULL ;


        while ( temp != NULL ) {

            path.push(temp) ;

            if ( name.compare(temp->data[0].key) < 0 ) { // if name < key
                // path.push(*(temp->link[0])) ; // go to the leftmost node
                temp = temp->link[0] ;
                cout << "left " ;
            }
            else if ( !name.compare(temp->data[0].key) ) { // if name == key01 (duplicate)
                cout << "dup" << path.size() ;
                return ;
            }
            else if ( temp->data[1].rSet.size() == 0 ) { // have two child
                cout << "right " ;
                // path.push(temp->link[2]) ; // go to the rightmost node
                temp = temp->link[2] ;
            }
            else { // have three child
                if ( name.compare(temp->data[1].key) < 0 ) { // if name > key01, but name < key02
                    // path.push(*(temp->link[1])) ; // go to the middle node
                    temp = temp->link[1] ;
                }
                else if ( !name.compare(temp->data[1].key) ) { // if name == key02 (duplicate)
                    return;
                }
                else {
                    if ( name.compare(temp->data[1].key) > 0 ) {
                        // path.push(*(temp->link[2])) ; // go to the rightmost node
                        temp = temp->link[2] ;
                    }
                } // else
            } // else


        } // while

    } // searchPath()

public:

    void insert23tree( int newRid, string newKey ) {

        // create a new slot
        // -------------------------------------------------------------
        slotType newSlot ;
        newSlot.rSet.push_back(newRid) ; // push new content of a record
        newSlot.key = newKey ;
        // -------------------------------------------------------------

        if ( root == NULL ) {
            root = CreatNode( NULL, NULL, NULL, newSlot ) ;
            cout << "build root success" << endl ;
        }
        else { // at least has one data

            nodeType *cur = root ;
            stack<nodePtr> path ;
            extraType blockup ;

            search( newKey, path ) ; // comparing same key

            if ( !path.empty() ) {

                cout << " search is OK " << endl ; // debug line

                cur = path.top() ; // leafnode


                // cout << "getTop is OK" << endl ; // debug line

                // case 1 it's duplicate key01 & key02 in the leafnode
                if ( cur->data[0].rSet.size() && !newKey.compare(cur->data[0].key) ) { // it's duplicate key01, so insert it directly
                    cur->data[0].rSet.push_back(newRid) ; // add a new record id
                    cout << "case1_1 OK" << endl ; // debug line
                }
                else if ( cur->data[1].rSet.size() && !newKey.compare(cur->data[1].key) ) { // it's duplicate key02, so insert it directly
                    cur->data[1].rSet.push_back(newRid) ; // add a new record id
                    cout << "case1_2 OK" << endl ; // debug line
                }
                // case 2 it's only one key in the leafnode and no duplicate
                else if ( cur->data[1].rSet.empty() ) { // at least one (rightmost) unused slot
                    insertLeaf( cur, newSlot ) ;


                    cout << "ref1 " << &(*root) << endl ; // debug line
                    cout << "ref2 " << &(*cur) << endl ; // debug line
                    cout << "case2 OK" << endl ; // debug line
                }
                // case 3 it's full of the leafnode
                else {
                    cout << "case3 OK" << endl ; // debug line
                    splitLeaf( cur, newSlot, blockup ) ; // split a leaf for an insertion

                    if ( cur->parent == NULL ) {// if a root is split
                        root = CreatRoot( cur, blockup.link, blockup.slot ) ;
                        cout << "case3 OK" << endl ; // debug line
                    }
                    else {
                        do {
                            path.pop() ;
                            cur = path.top() ;

                            // only one slot
                            if ( !cur->data[1].rSet.size() ) {
                                insertNonleaf( blockup, cur ) ; // add a slot into a non-leaf
                                break ;
                            }
                            // split a full non-leaf
                            else {
                                splitNonleaf( cur, blockup ) ;
                                if ( cur->parent == NULL ) {
                                    root = CreatRoot( cur, blockup.link, blockup.slot ) ;
                                    break ;
                                }  // if
                            }

                        } while( true ) ;

                    } // else

                } // else

            } // if

        } // else

    } // insert23tree()

    void PrintRoot() {
        cout << root->data[0].key << endl ;
    } // PrintRoot


}; // class Tree23

void DisplayMission() {
    cout << endl << "******  Max Heap and Deap  ******" ;
    cout << endl << "                                 " ;
    cout << endl << "* 0. QUIT                       *" ;
    cout << endl << "* 1. Build a 2-3 tree           *" ;
    cout << endl << "* 2. Build a DEAP               *" ;
    // cout << endl << "* 3.                            *" ;
    cout << endl << "                                 " ;
    cout << endl << "*********************************" ;
} // End displayMission

int main() {
    DM dataList ;
    int command ;


    do {
        DisplayMission() ;
        cout << endl << "Input a choice(0,1,2,3): " ;
        cin >> command ;
        cout << endl ;

        switch ( command ) {

            case 0:
                break ;
            case 1:


                if (dataList.ReadFile( "input" )) {
                    dataList.RemoveTitle() ;


                    TwoThreeTree tree23 ;

                    for( int i = 0 ; i < dataList.GetSize() ; i++ ) {
                        cout << i << " " << dataList.GetData( i, 1 ) << " insert Ok " << endl ; // debug line
                        tree23.insert23tree( i+1, dataList.GetData( i, 1 ) ) ;
                        cout << endl ;
                    } // for

                    cout << "<2-3 tree>" << endl ;
                    tree23.PrintRoot() ;

                } // if

                break ;

            case 2:
                /*
                if (dataList.ReadFile( "input" )) {
                    dataList.RemoveTitle() ;

                    Deap deap(dataList.GetSize()) ;

                    for( int i = 0 ; i < dataList.GetSize() ; i++ ) {
                        deap.DeapInsert( dataList.GetData( i, 6 ) ) ;
                    } // for

                    cout << "<DEAP>" << endl ;
                    deap.Print() ;

                } // if
                */

                break ;
            default :
                cout << "Command dose not exist!" << endl ;
        } // switch()

    } while ( command != 0 ) ;

    return 0;
}





