// �q���|�A 10626329 �K�۱s
// �ӳ]�T 10724243 ���V�X

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

typedef struct slot
{
    vector<int> rSet ; // a set of record id with the same key
    string key ; // record school name
} slotType ;

typedef struct nT   // 2-3 tree node
{
    slotType data[3] ; // 0 key01, 1 key02
    struct nT *link[4] ; // 0 leftmost, 1 middle, 2 rightmost
    struct nT *parent ; // point to the parent node
} nodeType ;

typedef struct n
{
    slotType data ;
    int height ;
    struct n *left ;
    struct n *right ;
} node;

class DM   // Data manage and In/Output
{

    vector< vector< string > > orinList;
    string fname ;

    void CombName( string name )
    {

        fname = name + fname.substr(5,3) ;
        fname = fname + ".txt" ;

    } // CombName()

    void Checkformat( string name )
    {
        // If input a code
        if ( fname.at(0) == '2' && fname.size() == 3 )
        {
            fname = name + fname + ".txt" ;
        } // if

    } // Checkformat()

    void ReadFileName()
    {
        cin >> fname ;
    } // ReadFileName()

public:

    string GetFileName()
    {
        return fname ;
    } // GetFileName()

    bool ReadFile( string fileType )
    {

        orinList.clear() ;

        fstream file ;
        char buffer ;
        string data ;
        vector<string> raw ;


        /*---------- deal with File ----------*/
        do
        {
            cout << "Input 201, 202, ...[0]Quit): " ;
            fname.clear() ;
            ReadFileName() ;

            if ( fname == "0" )
            {
                return false ;
            } // Quit to open file

            Checkformat( fileType ) ;

            cout << endl ;

            file.open( fname.c_str(), ios::in ) ; // open file

            if ( !file )
            {
                cout << "Error:Open file fail" << endl << endl ;
            } // if Error

        }
        while ( !file ) ;

        /*---------- deal with data ----------*/

        while ( file.get(buffer) )
        {

            if ( buffer != '\n' )
            {

                if ( buffer == '\t' )
                {
                    raw.push_back(data) ; // add new data on list
                    data.clear() ; // clear data
                }
                else
                {
                    data = data + buffer ; // complete the string
                } // else

            }
            else
            {
                raw.push_back(data) ; // add new data on list
                data.clear() ;
                orinList.push_back(raw) ;
                raw.clear() ;
            } // else

        } // end while

        if ( buffer != '\n' && !raw.empty() )
        {
            raw.push_back(data) ; // add new data on list
            data.clear() ;
            orinList.push_back(raw) ;
            raw.clear() ;
        } // end if



        if ( orinList.at(orinList.size()-1).at(0).compare("") == 0 && orinList.size() <= 5 )
        {
            orinList.pop_back() ;
        } // if only have a data but it display 2 and the second is empty

        file.close() ;

        return true ;

    } // ReadFile()

    bool fileExist()
    {
        if(!orinList.empty())
            return true;
        else return false;
    }

    void printdata()
    {
        int num = 1;
        for( int i = 0 ; i < orinList.size() ; i++ )
        {
            cout << num << " [" << i+1 << "]" << '\t' ;       // id
            cout << orinList.at(i).at(1) << ", " ;    // school name
            cout << orinList.at(i).at(3) << ", " ;    // department name
            cout << orinList.at(i).at(4) << ", " ;    // day/night
            cout << orinList.at(i).at(5) << ", " ;    // level
            cout << orinList.at(i).at(6) << ", " ;    // student num
            cout << endl ;
            num++ ;
        } // for
    }

//    bool PrintAll( )
//    {
//        if ( orinList.empty() )
//        {
//            return false ;
//        }
//
//        for ( int i = 0 ; i < orinList.size() ; i++ )
//        {
//            cout << "[" << i+1 << "] " ;
//            for ( int j = 0 ; j < orinList.at(i).size() ; j++ )
//            {
//                cout << orinList.at(i).at(j) << '\t' ;
//            }
//            cout << endl ;
//        }
//        return true ;
//    } // PrintAll()
//
//    void Write( string name )
//    {
//
//        fstream file ;
//
//        CombName( name ) ; // change fname to "ouput"
//
//        file.open( fname.c_str(), ios::out ) ;
//
//        for ( int i = 0 ; i < orinList.size() ; i++ )
//        {
//            for ( int position = 0 ; position < orinList.at(i).size() ; position++ )
//            {
//                file << orinList.at(i).at(position) << '\t' ;
//            }
//            file << '\n' ;
//        }
//
//        file.close();
//    } // Write()

    void RemoveTitle( )
    {
        orinList.erase(orinList.begin(), orinList.begin() + 3 ) ;
    } // RemoveTitle()

    string GetData( int pos, int num )
    {
        return orinList.at(pos).at(num) ;
    } // GetData

    int GetSize( )
    {
        return orinList.size() ;
    } // GetSize()

    void Clear()
    {
        if(!orinList.empty())
        {
            orinList.clear();
        }
    }

}; // Class FileIO

class TwoThreeTree
{

    nodeType *root ;
    int nodeCount ;

    nodeType *CreatNode( nodeType *left, nodeType *right, nodeType *pNode, slotType newSlot )
    {

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

        nodeCount++ ;

        return newNode;

    } // CreatNode()

    nodeType *search( string name )
    {
        nodeType *cur = root ;
        nodeType *prev = root ;

        cur->parent = NULL ;

        while ( cur != NULL )
        {

            // case 1 one key
            if ( !cur->data[0].rSet.empty() && cur->data[1].rSet.empty() )
            {

                // duplicate key01
                if ( !name.compare( cur->data[0].key ) )
                {
                    return cur ;
                }
                // newkey > key01
                else if ( name.compare( cur->data[0].key ) > 0 )
                {

                    prev = cur ;
                    cur = cur->link[1] ;
                }
                // newkey < key01
                else
                {
                    prev = cur ;
                    cur = cur->link[0] ;
                }
            }

            // case 2 two key
            else if ( !cur->data[0].rSet.empty() && !cur->data[1].rSet.empty() )
            {

                // duplicate key01
                if ( !name.compare( cur->data[0].key ) )
                {
                    return cur ;
                }
                // duplicate key02
                else if ( !name.compare( cur->data[1].key ) )
                {
                    return cur ;
                }
                // newkey < key01
                else if ( name.compare( cur->data[0].key ) < 0 )
                {
                    prev = cur ;
                    cur = cur->link[0] ;
                }
                // key01 < newkey < key02
                else if ( name.compare( cur->data[0].key ) > 0 && name.compare( cur->data[1].key ) < 0 )
                {
                    prev = cur ;
                    cur = cur->link[1] ;
                }
                // newkey > key02
                else
                {
                    prev = cur ;
                    cur = cur->link[2] ;
                }
            } // else if
        } // while
        return prev ;

    } // search()

    void Split( nodeType *cur, nodeType *left, nodeType *mid1, nodeType *mid2, nodeType *right )
    {

        nodeType *pNode ;

        if ( cur == root )
        {
            pNode = new nodeType ;
            nodeCount++ ; // add a new node
        }
        else
        {
            pNode = cur->parent ;
        }
        // if not a leaf

        nodeType *n1 = CreatNode( NULL, NULL, pNode, cur->data[0] ) ; // add a new node
        nodeType *n2 = CreatNode( NULL, NULL, pNode, cur->data[2] ) ; // add a new node

        if ( left != NULL && mid1 != NULL && mid2 != NULL && right != NULL )
        {
            n1->link[0] = left ;
            n1->link[1] = mid1 ;
            n2->link[0] = mid2 ;
            n2->link[1] = right ;

            left->parent = n1 ;
            mid1->parent = n1 ;
            mid2->parent = n2 ;
            right->parent = n2 ;

        } // if

        if ( cur == root )
        {
            pNode->data[0] = cur->data[1] ;
            pNode->link[0] = n1 ;
            pNode->link[1] = n2 ;
            root = pNode ;
        }

        // pNode have two key
        else if ( !pNode->data[1].rSet.empty() )
        {
            pNode->data[2] = cur->data[1] ;

            // check the pNode relationship is correct
            if ( pNode->data[0].key.compare( pNode->data[1].key ) > 0 )
            {
                swap( pNode->data[0], pNode->data[1] ) ;
            }

            //rightmost  [0][1][2]
            if ( pNode->data[2].key.compare( pNode->data[1].key ) > 0 )
            {

                Split( pNode, pNode->link[0], pNode->link[1], n1, n2 ) ;
            }
            // leftmost  [2][0][1]
            else if ( pNode->data[2].key.compare( pNode->data[0].key ) < 0 )
            {

                swap( pNode->data[2], pNode->data[1] ) ;
                swap( pNode->data[1], pNode->data[0] ) ;

                Split( pNode, n1, n2, pNode->link[1], pNode->link[2] ) ;
            }
            // middle  [0][2][1]
            else
            {

                swap( pNode->data[2], pNode->data[1] ) ;
                Split( pNode, pNode->link[0], n1, n2, pNode->link[2] ) ;
            }
        } // else if

        // pNode only have a key
        else if ( pNode->data[1].rSet.empty() )
        {

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

            if ( cur->data[1].key.compare( pNode->data[0].key ) < 0 )   // leftmost
            {

                swap( pNode->data[1], pNode->data[0] ) ;

                pNode->link[0] = n1 ; // insert link to n1
                pNode->link[2] = n2 ; // insert link to n2
                swap( pNode->link[1], pNode->link[2] ) ;

            }
            else     // rightmost
            {

                pNode->link[1] = n1 ; // insert link to n1
                pNode->link[2] = n2 ; // insert link to n2
            }

        } // else if

        delete cur ;
        nodeCount-- ; // delete a node

    } // Split()

public:

    TwoThreeTree( )
    {
        root = NULL ;
        nodeCount = 0 ;
    }

    void insert23tree( int newRid, string newKey )
    {

        // create a new slot
        // -------------------------------------------------------------
        slotType newSlot ;
        newSlot.rSet.push_back(newRid) ; // push new content of a record
        newSlot.key = newKey ;
        // -------------------------------------------------------------

        if ( root == NULL )
        {
            root = CreatNode( NULL, NULL, NULL, newSlot ) ;
        }
        else   // at least has one data
        {

            nodeType *cur = root ;

            cur = search( newKey ) ; // comparing same key

            if ( cur != NULL )
            {

                // case 1 one key
                if ( !cur->data[0].rSet.empty() && cur->data[1].rSet.empty() )
                {

                    // duplicate key01
                    if ( !newKey.compare( cur->data[0].key ) )
                    {
                        cur->data[0].rSet.push_back(newRid) ;
                    }
                    // newkey > key01
                    else if ( newKey.compare( cur->data[0].key ) > 0 )
                    {
                        cur->data[1] = newSlot ;
                    }
                    // newkey < key01
                    else
                    {
                        cur->data[1] = newSlot ;
                        swap( cur->data[0], cur->data[1] ) ;
                    }
                }
                // case 2 two key
                else if ( !cur->data[0].rSet.empty() && !cur->data[1].rSet.empty() )
                {


                    // duplicate key01
                    if ( !newKey.compare( cur->data[0].key ) )
                    {
                        cur->data[0].rSet.push_back(newRid) ;
                    }
                    // duplicate key02
                    else if ( !newKey.compare( cur->data[1].key ) )
                    {
                        cur->data[1].rSet.push_back(newRid) ;
                    }
                    // newkey < key01
                    else if ( newKey.compare( cur->data[0].key ) < 0 )
                    {
                        cur->data[2] = cur->data[1] ;
                        cur->data[1] = cur->data[0] ;
                        cur->data[0] = newSlot ;

                        Split( cur, NULL, NULL, NULL, NULL ) ;
                        // nodeCount++ ;
                    }
                    // key01 < newkey < key02
                    else if ( newKey.compare( cur->data[0].key ) > 0 && newKey.compare( cur->data[1].key ) < 0 )
                    {
                        cur->data[2] = cur->data[1] ;
                        cur->data[1] = newSlot ;

                        Split( cur, NULL, NULL, NULL, NULL ) ;
                        // nodeCount++ ;
                    }
                    // newkey > key02
                    else
                    {
                        cur->data[2] = newSlot ;

                        Split( cur, NULL, NULL, NULL, NULL ) ;
                        // nodeCount++ ;
                    }

                } // else if

            } // if

        } // else

    } // insert23tree()

    void PrintNodeCount()
    {
        cout << "number of node = " << nodeCount << endl ;
    } // PrintNodeCount()

    vector<int> GetRootID( int keyNum )
    {
        return root->data[keyNum].rSet ;
    } // GetRootID()

    void PrintRoot()
    {
        cout << root->data[0].key << endl ;
        if ( !root->data[1].rSet.empty() )
            cout << root->data[1].key << endl ;

    } // PrintRoot

    void free23Tree( nodeType* root01 )
    {
        if ( root01 != NULL )
        {
            free23Tree( root01->link[0] ) ;
            for ( int i = 0 ; i < KEY_NUM+1 ; i++ )
            {
                if ( root01->data[i].rSet.empty() )
                    break ;
                free23Tree( root01->link[i+1] ) ;
            } // for
            delete root01 ;
        } // if

    } // free23Tree()

    void Height()
    {
        int height = 0 ;
        nodeType * temp = root ;
        while ( temp!= NULL )
        {

            temp = temp->link[0] ;
            height++ ;
        }
        cout << "Tree height = " << height << endl ;

    } // Height ;

    void showPath( nodeType *cur )
    {
        while ( cur != NULL )
        {

            cout << "\t\t" << cur->data[0].key ;

            if ( !cur->data[1].rSet.empty() )
                cout << " " << cur->data[1].key ;

            cout << endl << endl ;

            cur = cur->parent ;
        }
    } // showPath()

    bool find( string name, vector<int> &rid )
    {
        nodeType *cur = search( name ) ;

        // key01 == name
        if ( !name.compare( cur->data[0].key ) )
        {
            rid = cur->data[0].rSet ;
            return true ;
        }
        // key02 == name
        else if ( !name.compare( cur->data[1].key ) )
        {
            rid = cur->data[1].rSet ;
            return true ;
        }

        return false;
    } // find()

}; // class Tree23

class AVLTree
{

    node *root ;
    int nodeCount ;

    node *CreatNode( node *left, node *right, int height, slotType newSlot )
    {

        node *newNode = NULL ;

        newNode = new node ;
        newNode->data.rSet = newSlot.rSet ;

        newNode->data.key = newSlot.key ;

        newNode->left = left ;
        newNode->right = right ;
        newNode->height = height ;
        nodeCount++ ;

        return newNode;


    } // CreatNode()

    node *insert( slotType newSlot, node *(&cur) )
    {

        if ( cur == NULL )
        {
            cur = CreatNode( NULL, NULL, 1, newSlot ) ;
            return cur ;
        }
        // newkey > cur		rightmost
        else if ( newSlot.key.compare( cur->data.key ) > 0 )
        {


            cur->right = insert( newSlot, cur->right ) ;

            /* -------------------- balance ----------------------*/
            if ( ( Getheight(cur->left) - Getheight(cur->right) ) <= -2 )
            {
                if ( newSlot.key.compare( (cur->right)->data.key ) < 0 )   // RL
                {
                    cur = RL( cur ) ;
                }
                else   // RR
                {
                    cur = RR( cur ) ;
                }
            } // if

        }
        // newkey < cur		leftmost
        else if ( newSlot.key.compare( cur->data.key ) < 0 )
        {


            cur->left = insert( newSlot, cur->left ) ;

            /* -------------------- balance ----------------------*/
            if ( ( Getheight(cur->left) - Getheight(cur->right) ) >= 2 )
            {
                if ( newSlot.key.compare( (cur->left)->data.key ) > 0 )   // LR
                {
                    cur = LR( cur ) ;
                }
                else   // LL
                {
                    cur = LL( cur ) ;
                }
            } // if

        }
        // newkey == cur	duplicate
        else
        {
            cur->data.rSet.push_back(newSlot.rSet.at(0)) ;
        } // else

        cur->height = Max( Getheight(cur->left), Getheight(cur->right) ) +1 ;
        return cur ;

    } // insert()


    int Max( int a, int b )
    {
        return ( a > b )? a : b ;
    } // Max()

    int Getheight( node *cur )
    {
        return ( cur == NULL ) ? 0 : cur->height ;
    } // Getheight()

    node *LL( node *k1 )
    {

        node *k2 = k1->left ;
        k1->left = k2->right ;
        k2->right = k1 ;

        k1->height = Max( Getheight(k1->left), Getheight(k1->right) ) +1 ;
        k2->height = Max( Getheight(k2->left), Getheight(k2->right) ) +1 ;

        return k2 ;

    } // LL()

    node *RR( node *k1 )
    {

        node *k2 = k1->right ;
        k1->right = k2->left ;
        k2->left = k1 ;

        k1->height = Max( Getheight(k1->left), Getheight(k1->right) ) +1 ;
        k2->height = Max( Getheight(k2->left), Getheight(k2->right) ) +1 ;

        return k2 ;

    } // RR()

    node *RL( node *k1 )
    {

        k1->right = LL( k1->right ) ;
        k1 = RR( k1 ) ;

        return k1 ;
    } // RL()

    node *LR( node *k1 )
    {

        k1->left = RR( k1->left ) ;
        k1 = LL( k1 ) ;

        return k1 ;
    } // LR()

public:

    AVLTree ()
    {
        root = NULL ;
        nodeCount = 0 ;
    }

    void insertAVLTree( int newRid, string newKey )
    {
        // create a new slot
        // -------------------------------------------------------------
        slotType newSlot ;
        newSlot.rSet.push_back(newRid) ; // push new content of a record
        newSlot.key = newKey ;
        // -------------------------------------------------------------

        if ( root == NULL )
        {
            root = CreatNode( NULL, NULL, 1, newSlot ) ;
        }
        else
        {

            insert( newSlot, root ) ;

        } // else

    } // insertAVLTree()

    vector<int> GetRootID( )
    {
        return root->data.rSet ;
    } // GetRootID()

    void PrintRoot()
    {
        cout << "root " << root->data.key << endl ;
    } // PrintRoot

    void Height()
    {
        cout << "height = " << root->height << endl ;
    } // PrintRoot

    void PrintNodeCount( )
    {
        cout << "number of node = " << nodeCount << endl ;
    } // PrintNodeCount()

    bool find( string name, vector<int> &rid )
    {

        node *cur = root ;

        while ( cur != NULL )
        {

            // name == key
            if ( !name.compare(cur->data.key) )
            {
                rid = cur->data.rSet ;
                return true ;
            }
            // name > key
            else if ( name.compare(cur->data.key) > 0 )
            {
                cur = cur->right ;
            }
            // name < key
            else
            {
                cur = cur->left ;
            }

        } // while

        return false ;

    } // find()

}; // class AVLTree

class Search
{
public:
    void DataSearch()
    {
        DM dataList;
        if (dataList.ReadFile("input"))
        {
            dataList.RemoveTitle();

            /*------------------------ build the tree -----------------------*/
            AVLTree avlTree;
            TwoThreeTree Tree23;

            for(int i = 0; i < dataList.GetSize(); i++)
            {
                avlTree.insertAVLTree(i+1, dataList.GetData(i, 3));
                Tree23.insert23tree(i+1, dataList.GetData(i, 1));
            } // for

            string key23, keyAVL;
            cout << "Enter a college name to search [*]: ";
            cin >> key23;
            cout << "Enter a department name to search [*]: ";
            cin >> keyAVL;
            cout << endl;
            vector<int> tree23DataPos, avlDataPos;

            /*--------------------- search the same name --------------------*/
            // AVL Tree Case
            if (!keyAVL.compare("*")); // skip search
            else if (avlTree.find(keyAVL, avlDataPos))
            {
                cout << "[info] Department searching Success! " << endl;
            }
            else
            {
                cout << "[info] Cannot find out the department name! " << endl;
            }

            // 2-3 Tree Case
            if (!key23.compare("*")) ; // skip search
            else if (Tree23.find(key23, tree23DataPos))
            {
                cout << "[info] College searching Success! " << endl;
            }
            else
            {
                cout << "[info] Cannot find out the college name! " << endl;
            }

            /*---------------------------------------------------------------*/

            // dont' care department and college
            if (!keyAVL.compare("*") && !key23.compare("*"))
            {
                dataList.printdata();
            }
            // dont' care college
            else if (!key23.compare("*"))
            {
                for(int i = 0; i < avlDataPos.size(); i++)
                {
                    cout << i+1 << " [" << avlDataPos.at(i) << "]" << " ";       // id
                    cout << dataList.GetData(avlDataPos.at(i)-1, 1) << ", ";    // school name
                    cout << dataList.GetData(avlDataPos.at(i)-1, 3) << ", ";    // department name
                    cout << dataList.GetData(avlDataPos.at(i)-1, 4) << ", ";    // day/night
                    cout << dataList.GetData(avlDataPos.at(i)-1, 5) << ", ";    // level
                    cout << dataList.GetData(avlDataPos.at(i)-1, 6) << '\t';    // student num
                    cout << endl;
                } // for
            }

            // dont' care department
            else if (!keyAVL.compare("*"))
            {
                for(int i = 0; i < tree23DataPos.size(); i++)
                {
                    cout << i+1 << " [" << tree23DataPos.at(i) << "]" << " ";       // id
                    cout << dataList.GetData(tree23DataPos.at(i)-1, 1) << ", ";    // school name
                    cout << dataList.GetData(tree23DataPos.at(i)-1, 3) << ", ";    // department name
                    cout << dataList.GetData(tree23DataPos.at(i)-1, 4) << ", ";    // day/night
                    cout << dataList.GetData(tree23DataPos.at(i)-1, 5) << ", ";    // level
                    cout << dataList.GetData(tree23DataPos.at(i)-1, 6) << '\t';    // student num
                    cout << endl;
                } // for
            }

            // same department name
            else
            {
                for(int i = 0; i < tree23DataPos.size(); i++)
                {
                    if (!dataList.GetData(tree23DataPos.at(i)-1, 3).compare(keyAVL))
                    {
                        cout << i+1 << " [" << tree23DataPos.at(i) << "]" << " ";       // id
                        cout << dataList.GetData(tree23DataPos.at(i)-1, 1) << ", ";    // school name
                        cout << dataList.GetData(tree23DataPos.at(i)-1, 3) << ", ";    // department name
                        cout << dataList.GetData(tree23DataPos.at(i)-1, 4) << ", ";    // day/night
                        cout << dataList.GetData(tree23DataPos.at(i)-1, 5) << ", ";    // level
                        cout << dataList.GetData(tree23DataPos.at(i)-1, 6) << '\t';    // student num
                        cout << endl;
                    } // if
                } // for
            } // else
        }
    }
};

void DisplayMission()
{
    cout << endl << "******  Max Heap and Deap  ******" ;
    cout << endl << "                                 " ;
    cout << endl << "* 0. QUIT                       *" ;
    cout << endl << "* 1. Build a 2-3 tree           *" ;
    cout << endl << "* 2. Build a AVL tree           *" ;
    cout << endl << "* 3. Intersection Query         *" ;
    cout << endl << "                                 " ;
    cout << endl << "*********************************" ;
} // End displayMission

int main()
{
    DM dataList ;
    int command ;

    do
    {
        DisplayMission() ;
        cout << endl << "Input a choice(0,1,2,3): " ;
        cin >> command ;
        cout << endl ;
        AVLTree avlTree ;

        switch ( command )
        {

        case 0:
            break ;
        case 1:
            dataList.Clear();
            if (dataList.ReadFile( "input" ))
            {
                dataList.RemoveTitle() ;

                TwoThreeTree tree23 ;

                for( int i = 0 ; i < dataList.GetSize() ; i++ )
                {

                    tree23.insert23tree( i+1, dataList.GetData( i, 1 ) ) ;

                } // for

                cout << "<2-3 tree>" << endl ;
                tree23.Height() ;
                tree23.PrintNodeCount() ;
                cout << "root data : " << endl ;
                // tree23.PrintRoot() ;

                vector<int> DataPos ;
                int num = 1 ;

                // key 0


                DataPos = tree23.GetRootID(1) ;
                if ( !DataPos.empty() )
                {
                    for( int i = 0; i < DataPos.size() ; i++ )
                    {
                        cout << num << "[" << DataPos.at(i) << "]" << " " ;        // id
                        cout << dataList.GetData( DataPos.at(i)-1, 1 ) << ", " ;    // school name
                        cout << dataList.GetData( DataPos.at(i)-1, 3 ) << ", " ;    // department name
                        cout << dataList.GetData( DataPos.at(i)-1, 4 ) << ", " ;    // day/night
                        cout << dataList.GetData( DataPos.at(i)-1, 5 ) << ", " ;    // level
                        cout << dataList.GetData( DataPos.at(i)-1, 6 ) << '\t' ;    // student num
                        cout << endl ;
                        num++ ;
                    } // for
                } // if

                DataPos = tree23.GetRootID(0) ;
                for( int i = 0; i < DataPos.size() ; i++ )
                {
                    cout << num << " [" << DataPos.at(i) << "]" << " " ;       // id
                    cout << dataList.GetData( DataPos.at(i)-1, 1 ) << ", " ;    // school name
                    cout << dataList.GetData( DataPos.at(i)-1, 3 ) << ", " ;    // department name
                    cout << dataList.GetData( DataPos.at(i)-1, 4 ) << ", " ;    // day/night
                    cout << dataList.GetData( DataPos.at(i)-1, 5 ) << ", " ;    // level
                    cout << dataList.GetData( DataPos.at(i)-1, 6 ) << '\t' ;    // student num
                    cout << endl ;
                    num++ ;
                } // for
            } // if

            break ;

        case 2:
            if(dataList.fileExist())
            {
                for( int i = 0 ; i < dataList.GetSize() ; i++ )
                {
                    // cout << "[" << i+1 << "] " << dataList.GetData( i, 3 ) << " " ;
                    avlTree.insertAVLTree( i+1, dataList.GetData( i, 3 ) ) ;

                } // for

                cout << "<AVL tree>" << endl ;
                avlTree.Height() ;
                avlTree.PrintNodeCount() ;
                cout << "root data : " << endl ;
                // avlTree.PrintRoot() ;


                vector<int> DataPos ;
                int num = 1 ;

                // key 0
                DataPos = avlTree.GetRootID( ) ;
                for( int i = 0; i < DataPos.size() ; i++ )
                {
                    cout << num << " [" << DataPos.at(i) << "]" << " " ;       // id
                    cout << dataList.GetData( DataPos.at(i)-1, 1 ) << ", " ;    // school name
                    cout << dataList.GetData( DataPos.at(i)-1, 3 ) << ", " ;    // department name
                    cout << dataList.GetData( DataPos.at(i)-1, 4 ) << ", " ;    // day/night
                    cout << dataList.GetData( DataPos.at(i)-1, 5 ) << ", " ;    // level
                    cout << dataList.GetData( DataPos.at(i)-1, 6 ) << '\t' ;    // student num
                    cout << endl ;
                    num++ ;
                } // for

            }// if
            else
                cout << endl<<"----- Execute Mission 1 first! -----"<<endl;

            break ;
        case 3:
            Search Dsearch;
            Dsearch.DataSearch();
            break ;

        default :
            cout << "Command dose not exist!" << endl ;
        } // switch()

        cout << endl ;

    }
    while ( command != 0 ) ;

    return 0;
}
