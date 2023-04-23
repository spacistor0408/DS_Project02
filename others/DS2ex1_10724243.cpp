
// 10724243 石顥澐

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

typedef struct pointT { // a point on the search path
    nodeType *pnode ; // pointer to parent node
    int pidx ; // entrance index on the parent node
} pointType ;

typedef struct bT { // temp
    slotType slot ; // a pair of ( record id, key )
    nodeType *link ; // a pointer to a child on th right
} blockType ;


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

    /*
    void insert23tree();
    nodeType *createNode();
    void searchPath();
    void insertLeaf();
    void splitLeaf();
    nodeType *createRoot();
    void insertNonleaf();
    void splitNonleaf();
    void free23tree( nodeType *& );

    */

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

    void searchPath( string name, stack<pointType> path ) {

        nodeType *cur = root ;
        pointType oneP ;
        int pos ;

        while ( cur != NULL ) {

            oneP.pnode = cur ; // parent node of oneP is cur

            for ( pos = 0 ; pos < KEY_NUM ; pos++ ) {
                if ( !cur->data[pos].rSet.size() || name.compare( cur->data[pos].key ) < 0 ) { // if unused slot, name > key or name < key
                    break ;
                } else if ( !name.compare(cur->data[pos].key) ) { // if name == key (duplicate)
                    oneP.pidx = pos ;
                    path.push(oneP) ;
                    return ;
                } // else if
            } // for

            oneP.pidx = pos ; // keep track of the pointer
            path.push(oneP) ; // visited node ()
            cur = cur->link[pos] ; // recursive search at the next level

        } // while

    } // searchPath()

    void insertLeaf( slotType newS, pointType aLeaf ) {

        for ( int i = KEY_NUM-1 ; i >= aLeaf.pidx ; i-- ) {
            if ( i > aLeaf.pidx ) { // shift an existing record to the right

                aLeaf.pnode->data[i].rSet = aLeaf.pnode->data[i-1].rSet ;
                aLeaf.pnode->data[i].key = aLeaf.pnode->data[i-1].key ;

            } else if ( i == aLeaf.pidx ) {

                aLeaf.pnode->data[i].rSet = newS.rSet ; // save the new record in a new slot
                aLeaf.pnode->data[i].key = newS.key ;
            }
            else
                break ;
        } // for

    } // insertLeaf()

    void splitLeaf( slotType newSlot, pointType aLeaf, blockType aBlock ) {
        slotType buf[PTR_NUM] ;
        int idx = 0 ;

        for ( int i = 0 ; i<PTR_NUM ; i++ ) {
            buf[i].rSet = (i==aLeaf.pidx) ? newSlot.rSet : aLeaf.pnode->data[idx].rSet ;
            buf[i].key = (i==aLeaf.pidx) ? newSlot.key : aLeaf.pnode->data[idx++].key ;
        } // for

        aLeaf.pnode->data[0].rSet = buf[0].rSet ;
        aLeaf.pnode->data[0].key = buf[0].key ;
        for ( int i = 1 ; i < KEY_NUM ; i++ )
            aLeaf.pnode->data[i].rSet.clear() ;
        aBlock.link = CreatNode( NULL, NULL, aLeaf.pnode->parent, buf[2] ) ;

        aBlock.slot.rSet = buf[1].rSet ;
        aBlock.slot.key = buf[1].key ;

    } // splitLeaf()

    nodeType *CreatRoot( nodeType *left, nodeType *right, slotType oneSlot ) {
        nodeType *newRoot = CreatNode( left, right, NULL, oneSlot ) ;
        left->parent = newRoot ;
        right->parent = newRoot ;

        return newRoot ;
    } // CreatRoot()

    void insertNonleaf( blockType oneB, pointType goal ) {
        goal.pnode->data[1] = oneB.slot ;
        goal.pnode->link[2] = oneB.link ;

        if ( goal.pnode->data[0].key.compare( oneB.slot.key ) > 0 ) { // if key01 > new key
            swap( goal.pnode->data[0], goal.pnode->data[1] ) ;
            swap( goal.pnode->link[1], goal.pnode->link[2] ) ;
        } // if

    } // insertNonleaf()

    void splitNonleaf( pointType goal, blockType oneB ) {
        slotType buf[PTR_NUM] ;
        nodeType *childBuf[PTR_NUM+1] ; // a buffer to keep pointers of children

        buf[0] = goal.pnode->data[0] ;
        buf[1] = goal.pnode->data[1] ;
        buf[2] = oneB.slot ;

        // sort the buf
        if ( buf[2].key.compare( buf[1].key ) < 0 )
            swap( buf[2], buf[1] ) ;
        if ( buf[1].key.compare( buf[0].key ) < 0 )
            swap( buf[1], buf[0] ) ;

        // sort the childBuf
        childBuf[0] = goal.pnode->link[0] ;
        childBuf[1] = goal.pnode->link[1] ;
        childBuf[2] = goal.pnode->link[2] ;
        childBuf[3] = oneB.link ;

        if ( childBuf[3]->data[0].key.compare( childBuf[2]->data[0].key ) < 0 )
            swap( childBuf[3], childBuf[2] ) ;
        if ( childBuf[2]->data[0].key.compare( childBuf[1]->data[0].key ) < 0 )
            swap( childBuf[2], childBuf[1] ) ;
        if ( childBuf[1]->data[0].key.compare( childBuf[0]->data[0].key ) < 0 )
            swap( childBuf[1], childBuf[0] ) ;

        // ------------------------------------------------------------------

        goal.pnode->data[0] = buf[0] ;
        goal.pnode->link[0] = childBuf[0] ;
        goal.pnode->link[1] = childBuf[1] ;

        nodeType *sibling = CreatNode( childBuf[2], childBuf[3], goal.pnode->parent, buf[2] ) ;

        oneB.slot = buf[1] ;
        oneB.link = sibling ;

    } // splitNonleaf()
public:

    TwoThreeTree() { root = NULL ; }

    void insert23tree( int newRid, string newKey ) {

        slotType newSlot ;
        newSlot.rSet.push_back(newRid) ; // push new content of a record
        newSlot.key = newKey ;

        if ( root == NULL ) {
            root = CreatNode( NULL, NULL, NULL, newSlot ) ;
        }
        else {
            stack<pointType> aPath ; // record the path
            pointType curP ;
            blockType blockUp ; // a data received from a split

            searchPath( newKey, aPath ) ; // comparing same key

            if ( !aPath.empty() ) {

                curP = aPath.top() ; // leafNode or internalNode

                // case 1 it's duplicate or empty in the node
                if ( curP.pnode->data[curP.pidx].rSet.size() && !newKey.compare(curP.pnode->data[curP.pidx].key) ) // it's duplicate key, so insert it directly

                    curP.pnode->data[curP.pidx].rSet.push_back(newRid) ; // add a new record id

                // case 2 it's only one key in the node
                else if ( !curP.pnode->data[KEY_NUM-1].rSet.size() ) // at least one (rightmost) unused slot

                    insertLeaf( newSlot, curP ) ;

                // case 3 it's full of the node
                else {

                    splitLeaf( newSlot, curP, blockUp ) ;

                    if ( curP.pnode->parent == NULL ) // if a root is split
                        root = CreatRoot( curP.pnode, blockUp.link, blockUp.slot ) ;
                    else {
                        do {
                            aPath.pop() ;
                            curP = aPath.top() ;

                            // only one slot
                            if ( !curP.pnode->data[1].rSet.size() ) {
                                insertNonleaf( blockUp, curP ) ; // add a slot into a non-leaf
                                break ;
                            }
                            // split a full non-leaf
                            else {
                                splitNonleaf( curP, blockUp ) ;
                                if ( curP.pnode->parent == NULL ) {
                                    root = CreatRoot( curP.pnode, blockUp.link, blockUp.slot ) ;

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
        cout << root->data[0].rSet.at(0) << " " << root->data[0].key << endl ;
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
                        // cout << i << " " << dataList.GetData( i, 1 ) << " insert Ok " << endl ; // debug line
                        tree23.insert23tree( i+1, dataList.GetData( i, 1 ) ) ;
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





