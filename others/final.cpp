// 電機四乙 10626329 袁倫御
// 商設三 10724243 石顥澐

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
    slotType data[3] ; // 0 key01, 1 key02
    struct nT *link[4] ; // 0 leftmost, 1 middle, 2 rightmost
    struct nT *parent ; // point to the parent node
} nodeType ;

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
        newNode->data[2].rSet.clear() ;

        newNode->data[0].key = newSlot.key ;
        newNode->data[1].key = "" ;
        newNode->data[2].key = "" ;

        newNode->parent = pNode ;
        newNode->link[0] = left ;
        newNode->link[1] = right ;
        newNode->link[2] = NULL ;
        newNode->link[3] = NULL ;

        return newNode;

    } // CreatNode()

    nodeType *search( string name ) {

        nodeType *cur = root ;
        nodeType *prev = root ;

        cur->parent = NULL ;


        while ( cur != NULL ) {

            // case 1 one key
            if ( !cur->data[0].rSet.empty() && cur->data[1].rSet.empty() ) {

                // duplicate key01
                if ( !name.compare( cur->data[0].key ) ) {
                    return cur ;
                }
                // newkey > key01
                else if ( name.compare( cur->data[0].key ) > 0 ) {
                    prev = cur ;
                    cur = cur->link[1] ;
                }
                // newkey < key01
                else {
                    prev = cur ;
                    cur = cur->link[0] ;
                }
            }
            // case 2 two key
            else if ( !cur->data[0].rSet.empty() && !cur->data[1].rSet.empty() ) {

                // duplicate key01
                if ( !name.compare( cur->data[0].key ) ) {
                    return cur ;
                }
                // duplicate key02
                else if ( !name.compare( cur->data[1].key ) ) {
                    return cur ;
                }
                // newkey < key01
                else if ( name.compare( cur->data[0].key ) < 0 ) {
                    prev = cur ;
                    cur = cur->link[0] ;
                }
                // key01 < newkey < key02
                else if ( name.compare( cur->data[0].key ) > 0 && name.compare( cur->data[1].key ) < 0 ) {
                    prev = cur ;
                    cur = cur->link[1] ;
                }
                // newkey > key02
                else {
                    prev = cur ;
                    cur = cur->link[2] ;
                }
            } // else if
        } // while
        return prev ;

    } // search()

    void Split( nodeType *cur, nodeType *left, nodeType *mid1, nodeType *mid2, nodeType *right ) {

        nodeType *pNode ;

        if ( cur == root ) {
            pNode = new nodeType ;
        } else {
            pNode = cur->parent ;
        }
        // if not a leaf

        nodeType *n1 = CreatNode( NULL, NULL, pNode, cur->data[0] ) ;
        nodeType *n2 = CreatNode( NULL, NULL, pNode, cur->data[2] ) ;

        if ( left != NULL && mid1 != NULL && mid2 != NULL && right != NULL ) {
            n1->link[0] = left ;
            n1->link[1] = mid1 ;
            n2->link[0] = mid2 ;
            n2->link[1] = right ;

            left->parent = n1 ;
            mid1->parent = n1 ;
            mid2->parent = n2 ;
            right->parent = n2 ;

        } // if

        if ( cur == root ) {
            pNode->data[0] = cur->data[1] ;
            pNode->link[0] = n1 ;
            pNode->link[1] = n2 ;
            root = pNode ;
        }
        // pNode have two key
        else if ( !pNode->data[1].rSet.empty() ) {
            pNode->data[2] = cur->data[1] ;

            // check the pNode relationship is correct
            if ( pNode->data[0].key.compare( pNode->data[1].key ) > 0 ) {
                swap( pNode->data[0], pNode->data[1] ) ;
            }


            //rightmost  [0][1][2]
            if ( pNode->data[2].key.compare( pNode->data[1].key ) > 0 ) {

                Split( pNode, pNode->link[0], pNode->link[1], n1, n2 ) ;

            }
            // leftmost  [2][0][1]
            else if ( pNode->data[2].key.compare( pNode->data[0].key ) < 0 ) {

                swap( pNode->data[2], pNode->data[1] ) ;
                swap( pNode->data[1], pNode->data[0] ) ;

                Split( pNode, n1, n2, pNode->link[1], pNode->link[2] ) ;

            }
            // middle  [0][2][1]
            else {

                swap( pNode->data[2], pNode->data[1] ) ;
                Split( pNode, pNode->link[0], n1, n2, pNode->link[2] ) ;
            }

        } // else if
        // pNode only have a key
        else if ( pNode->data[1].rSet.empty() ) {

            pNode->data[1] = cur->data[1] ; // add to key02
            // [1] < [0]


            //   oringin
            //
            //    [  ]
            //   /    \
            // [0]    [1]

            //     case1             case2
            //
            //    [  |  ]           [  |  ]
            //   /     \ \         /  /    \
            // [0]    [1][2]  or [1][2]    [0]

            if ( cur->data[1].key.compare( pNode->data[0].key ) < 0 ) { // leftmost

                swap( pNode->data[1], pNode->data[0] ) ;

                pNode->link[0] = n1 ; // insert link to n1
                pNode->link[2] = n2 ; // insert link to n2
                swap( pNode->link[1], pNode->link[2] ) ;

            } else { // rightmost

                pNode->link[1] = n1 ; // insert link to n1
                pNode->link[2] = n2 ; // insert link to n2
            }

        } // else if

    } // Split()

public:

    TwoThreeTree( ) { root = NULL ; }

    void insert23tree( int newRid, string newKey ) {

        // create a new slot
        // -------------------------------------------------------------
        slotType newSlot ;
        newSlot.rSet.push_back(newRid) ; // push new content of a record
        newSlot.key = newKey ;
        // -------------------------------------------------------------

        if ( root == NULL ) {
            root = CreatNode( NULL, NULL, NULL, newSlot ) ;
        }
        else { // at least has one data

            nodeType *cur = root ;

            cur = search( newKey ) ; // comparing same key

            if ( cur != NULL ) {

                // case 1 one key
                if ( !cur->data[0].rSet.empty() && cur->data[1].rSet.empty() ) {

                    // duplicate key01
                    if ( !newKey.compare( cur->data[0].key ) ) {
                        cur->data[0].rSet.push_back(newRid) ;
                    }
                    // newkey > key01
                    else if ( newKey.compare( cur->data[0].key ) > 0 ) {
                        cur->data[1] = newSlot ;
                    }
                    // newkey < key01
                    else {
                        cur->data[1] = newSlot ;
                        swap( cur->data[0], cur->data[1] ) ;
                    }
                }
                // case 2 two key
                else if ( !cur->data[0].rSet.empty() && !cur->data[1].rSet.empty() ) {


                    // duplicate key01
                    if ( !newKey.compare( cur->data[0].key ) ) {
                        cur->data[0].rSet.push_back(newRid) ;
                    }
                    // duplicate key02
                    else if ( !newKey.compare( cur->data[1].key ) ) {
                        cur->data[1].rSet.push_back(newRid) ;
                    }
                    // newkey < key01
                    else if ( newKey.compare( cur->data[0].key ) < 0 ) {
                        cur->data[2] = cur->data[1] ;
                        cur->data[1] = cur->data[0] ;
                        cur->data[0] = newSlot ;

                        Split( cur, NULL, NULL, NULL, NULL ) ;
                    }
                    // key01 < newkey < key02
                    else if ( newKey.compare( cur->data[0].key ) > 0 && newKey.compare( cur->data[1].key ) < 0 ) {
                        cur->data[2] = cur->data[1] ;
                        cur->data[1] = newSlot ;

                        Split( cur, NULL, NULL, NULL, NULL ) ;
                    }
                    // newkey > key02
                    else {
                        cur->data[2] = newSlot ;

                        Split( cur, NULL, NULL, NULL, NULL ) ;
                    }

                } // else if

            } // if

        } // else

    } // insert23tree()

    vector<int> GetRootID( int keyNum ) {
        return root->data[keyNum].rSet ;
    } // GetRootID()

    void PrintRoot() {
        cout << root->data[0].key << endl ;
        if ( !root->data[1].rSet.empty() )
            cout << root->data[1].key << endl ;

    } // PrintRoot

    void free23Tree( nodeType* root01 ) {
        if ( root01 != NULL ) {
            free23Tree( root01->link[0] ) ;
            for ( int i = 0 ; i < KEY_NUM+1 ; i++ ) {
                if ( root01->data[i].rSet.empty() )
                    break ;
                free23Tree( root01->link[i+1] ) ;
            } // for
            delete root01 ;
        } // if

    } // free23Tree()

    void Height(  ) {
        int height = 0 ;
        nodeType * temp = root ;
        while ( temp!= NULL ) {

            temp = temp->link[0] ;
            height++ ;
        }
        cout << "Tree height = " << height << endl ;

    } // Height ;

    void showPath( nodeType *cur ) {
        while ( cur != NULL ) {

            cout << "\t\t" << cur->data[0].key ;

            if ( !cur->data[1].rSet.empty() )
                cout << " " << cur->data[1].key ;

            cout << endl << endl ;

            cur = cur->parent ;
        }
    } // showPath()

}; // class Tree23

void DisplayMission() {
    cout << endl << "******  Max Heap and Deap  ******" ;
    cout << endl << "                                 " ;
    cout << endl << "* 0. QUIT                       *" ;
    cout << endl << "* 1. Build a 2-3 tree           *" ;
    cout << endl << "* 2. Build a AVL tree           *" ;
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

                        tree23.insert23tree( i+1, dataList.GetData( i, 1 ) ) ;

                    } // for

                    cout << "<2-3 tree>" << endl ;
                    tree23.Height() ;
                    cout << endl << "root : " << endl ;
                    // tree23.PrintRoot() ;

                    vector<int> DataPos ;
                    int num = 1 ;

                    // key 0
                    DataPos = tree23.GetRootID(0) ;
                    for( int i = 0; i < DataPos.size() ; i++ ) {
                        cout << num << " [" << DataPos.at(i) << "]" << '\t' ;       // id
                        cout << dataList.GetData( DataPos.at(i)-1, 1 ) << '\t' ;    // school name
                        cout << dataList.GetData( DataPos.at(i)-1, 3 ) << '\t' ;    // department name
                        cout << dataList.GetData( DataPos.at(i)-1, 4 ) << '\t' ;    // day/night
                        cout << dataList.GetData( DataPos.at(i)-1, 5 ) << '\t' ;    // level
                        cout << dataList.GetData( DataPos.at(i)-1, 6 ) << '\t' ;    // student num
                        cout << endl ;
                        num++ ;
                    } // for

                    DataPos = tree23.GetRootID(1) ;
                    if ( !DataPos.empty() ) {
                        for( int i = 0; i < DataPos.size() ; i++ ) {
                            cout << num << "[" << DataPos.at(i) << "]" << '\t' ;        // id
                            cout << dataList.GetData( DataPos.at(i)-1, 1 ) << '\t' ;    // school name
                            cout << dataList.GetData( DataPos.at(i)-1, 3 ) << '\t' ;    // department name
                            cout << dataList.GetData( DataPos.at(i)-1, 4 ) << '\t' ;    // day/night
                            cout << dataList.GetData( DataPos.at(i)-1, 5 ) << '\t' ;    // level
                            cout << dataList.GetData( DataPos.at(i)-1, 6 ) << '\t' ;    // student num
                            cout << endl ;
                            num++ ;
                        } // for
                    } // if


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





