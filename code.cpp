/**
 * Implementujte metodu insert (vkládání), erase (mazání prvků
 * ze stromu a zároveň je potřeba uchovat správné pořádí podle
 * data vkládání) a vlastní destruktor, ostatní metody jsou
 * naimplentované. Kopírující konstruktor i operátor přiřazení
 * zakázán. Není povolené si přidat žádnou členskou proměnnou,
 * ale lze naimplementovat vlastní pomocné metody.
 *
 * Za správnost šablony neručím :-).
 */
 
#include <string>
#include <cassert>
#include <iostream>
 
using namespace std;
 
class CTree
{
  public:
    CTree() = default;

    ~CTree()
    {
        //todo
        CNode * n = m_First;
        CNode * p = nullptr;
        while (n)
        {
            p = n->m_NextOrder;
            delete n;
            n = p;

        }
    }

    CTree(const CTree &src) = delete;
    bool operator=(const CTree &other) = delete;

    bool isSet(string key)
    {
      CNode * n = m_Root;
      while (n)
      {
        if (key == n->m_Key)
          return true;
        else if (key > n->m_Key)
          n = n->m_R;
        else
          n = n->m_L;
      }
      return false;
    }

    bool insert(string key)
    {
      //todo
      if(m_Root == nullptr){
        m_Root = n_add(key);
        return true;
      }
      CNode * currNode = m_Root;
      while(currNode){
        if(s_comp(key, currNode->m_Key)){ // go right
            if(currNode->m_R){
                currNode = currNode->m_R;
                continue;
            }
            currNode->m_R = n_add(key);
            break;
        }
        else if(s_comp(currNode->m_Key, key)){ // go left
            if(currNode->m_L){
                currNode = currNode->m_L;
                continue;
            }
            currNode->m_L = n_add(key);
            break;
        }
        else{
            return false; // already is in it
        }
      }

      return true;
      
    }

    bool erase(string key)
    {
      //todo
      if(m_Root == nullptr){
        return false;
      }
      CNode * currNode = m_Root;
      CNode * currNodeParent = currNode;
      while(currNode){
        if(s_comp(key, currNode->m_Key)){ // go right
            if(currNode->m_R){
                currNodeParent = currNode;
                currNode = currNode->m_R;
                continue;
            }
            break;
        }
        else if(s_comp(currNode->m_Key, key)){ // go left
            if(currNode->m_L){
                currNodeParent = currNode;
                currNode = currNode->m_L;
                continue;
            }
            break;
        }
        else{ // FOUND !!!
            if(currNode->m_L == nullptr && currNode->m_R == nullptr){ // 0 children
                if(currNodeParent == currNode){
                    m_Root = nullptr;
                }
                else{
                    if(currNodeParent->m_L == currNode){
                        currNodeParent->m_L = nullptr;
                    }
                    else{
                        currNodeParent->m_R = nullptr;
                    }
                }
                n_delete(currNode);
            }
            else if( (currNode->m_L && currNode->m_R == nullptr) || (currNode->m_R && currNode->m_L == nullptr)){  // 1 child
                if(currNode->m_L){
                    if(currNodeParent == currNode){
                        m_Root = currNode->m_L;
                    }
                    else{
                        if(currNodeParent->m_L == currNode){
                            currNodeParent->m_L = currNode->m_L;
                        }
                        else{
                            currNodeParent->m_R = currNode->m_L;
                        }
                    }
                }
                else{
                    if(currNodeParent == currNode){
                        m_Root = currNode->m_R;
                    }
                    else{
                        if(currNodeParent->m_L == currNode){
                            currNodeParent->m_L = currNode->m_R;
                        }
                        else{
                            currNodeParent->m_R = currNode->m_R;
                        }
                    }
                    
                }
                n_delete(currNode);
            }
            else{ // 2 children

                CNode * smallestNode = currNode->m_R;
                CNode * smallestNodeParent = currNode;
                bool deleteLeftFlag = false;
                while(smallestNode->m_L){
                    deleteLeftFlag = true;
                    smallestNodeParent = smallestNode;
                    smallestNode = smallestNode->m_L;
                }

                if(currNodeParent == currNode){
                    m_Root = smallestNode;
                }
                else{
                    if(currNodeParent->m_L == currNode){
                        currNodeParent->m_L = smallestNode;
                    }
                    else{
                        currNodeParent->m_R = smallestNode;
                    }
                }
                
                smallestNode->m_L = currNode->m_L;
                
                if(deleteLeftFlag){
                    smallestNode->m_R = currNode->m_R;
                    smallestNodeParent->m_L = nullptr;
                }
                n_delete(currNode);

            }
            return true; // erased
        }
      }
      return false;

    }

  protected:
    class CNode
    {
      public:
        CNode(string key) : m_Key(key) {}
        string m_Key;
        CNode * m_L = nullptr;
        CNode * m_R = nullptr;
        CNode * m_PrevOrder = nullptr;
        CNode * m_NextOrder = nullptr;
    };
    CNode * m_Root = nullptr;
    CNode * m_First = nullptr;
    CNode * m_Last = nullptr;

  private:
    bool s_comp(string s1, string s2){
        //todo
        return s1 > s2;
    }

    CNode * n_add(string key){
        CNode * res = new CNode(key);

        if(!m_First){
            m_First = res;
            m_Last = res;
            return res;
        }

        m_Last->m_NextOrder = res;
        res->m_PrevOrder = m_Last;
        m_Last = m_Last->m_NextOrder;
        
        return res;
    }

    bool n_delete(CNode * node){
        if(node == m_First || node == m_Last){
            if(node == m_First){
                m_First = node->m_NextOrder;
                if(m_First){
                    m_First->m_PrevOrder = nullptr;
                }
            }
            if(node == m_Last){
                m_Last = node->m_PrevOrder;
                if(m_Last){
                    m_Last->m_NextOrder = nullptr;
                }
            }
        }
        else{
            node->m_PrevOrder->m_NextOrder = node->m_NextOrder;
            node->m_NextOrder->m_PrevOrder = node->m_PrevOrder;
        }

        delete node;

        return true;
    }
};
 
class CTester : public CTree
{
  public:
    CTester () = default;
    void test()
    {
      CTester t0;
      assert(t0.insert("PA1")==true);
      assert(t0.m_First->m_Key=="PA1" && t0.m_First->m_NextOrder == nullptr);
      assert(t0.isSet("PA1")==true);
      assert(t0.insert("UOS")==true);
      assert(t0.insert("PA2")==true);
      assert(t0.isSet("PA2")==true);
      assert(t0.isSet("PA3")==false);
 
      assert(t0.insert("PA2")==false);
      assert(t0.insert("CAO")==true);
      assert(t0.insert("LIN")==true);
      assert(t0.insert("AAG")==true);
      assert(t0.insert("AG1")==true);
      assert(t0.insert("ZDM")==true);
 
      assert(t0.m_First->m_Key == "PA1"
             && t0.m_First->m_NextOrder->m_Key == "UOS"
             && t0.m_First->m_NextOrder->m_NextOrder->m_Key == "PA2"
             && t0.m_First->m_NextOrder->m_NextOrder->m_NextOrder->m_Key == "CAO"
             && t0.m_First->m_NextOrder->m_NextOrder->m_NextOrder->m_NextOrder->m_Key == "LIN"
             && t0.m_First->m_NextOrder->m_NextOrder->m_NextOrder->m_NextOrder->m_NextOrder->m_Key == "AAG"
             && t0.m_First->m_NextOrder->m_NextOrder->m_NextOrder->m_NextOrder->m_NextOrder->m_NextOrder->m_Key == "AG1"
             && t0.m_First->m_NextOrder->m_NextOrder->m_NextOrder->m_NextOrder->m_NextOrder->m_NextOrder->m_NextOrder->m_Key == "ZDM");
 
      assert(t0.m_Last->m_Key == "ZDM"
             && t0.m_Last->m_PrevOrder->m_Key == "AG1"
             && t0.m_Last->m_PrevOrder->m_PrevOrder->m_Key == "AAG"
             && t0.m_Last->m_PrevOrder->m_PrevOrder->m_PrevOrder->m_Key == "LIN"
             && t0.m_Last->m_PrevOrder->m_PrevOrder->m_PrevOrder->m_PrevOrder->m_Key == "CAO"
             && t0.m_Last->m_PrevOrder->m_PrevOrder->m_PrevOrder->m_PrevOrder->m_PrevOrder->m_Key == "PA2"
             && t0.m_Last->m_PrevOrder->m_PrevOrder->m_PrevOrder->m_PrevOrder->m_PrevOrder->m_PrevOrder->m_Key == "UOS"
             && t0.m_Last->m_PrevOrder->m_PrevOrder->m_PrevOrder->m_PrevOrder->m_PrevOrder->m_PrevOrder->m_PrevOrder->m_Key == "PA1");
 
      assert(t0.erase("")==false);
 
      assert(t0.erase("ZDM")==true);
      assert(t0.m_First->m_Key == "PA1"
             && t0.m_First->m_NextOrder->m_Key == "UOS"
             && t0.m_First->m_NextOrder->m_NextOrder->m_Key == "PA2"
             && t0.m_First->m_NextOrder->m_NextOrder->m_NextOrder->m_Key == "CAO"
             && t0.m_First->m_NextOrder->m_NextOrder->m_NextOrder->m_NextOrder->m_Key == "LIN"
             && t0.m_First->m_NextOrder->m_NextOrder->m_NextOrder->m_NextOrder->m_NextOrder->m_Key == "AAG"
             && t0.m_First->m_NextOrder->m_NextOrder->m_NextOrder->m_NextOrder->m_NextOrder->m_NextOrder->m_Key == "AG1");
      assert(t0.m_Last->m_Key == "AG1"
             && t0.m_Last->m_PrevOrder->m_Key == "AAG"
             && t0.m_Last->m_PrevOrder->m_PrevOrder->m_Key == "LIN"
             && t0.m_Last->m_PrevOrder->m_PrevOrder->m_PrevOrder->m_Key == "CAO"
             && t0.m_Last->m_PrevOrder->m_PrevOrder->m_PrevOrder->m_PrevOrder->m_Key == "PA2"
             && t0.m_Last->m_PrevOrder->m_PrevOrder->m_PrevOrder->m_PrevOrder->m_PrevOrder->m_Key == "UOS"
             && t0.m_Last->m_PrevOrder->m_PrevOrder->m_PrevOrder->m_PrevOrder->m_PrevOrder->m_PrevOrder->m_Key == "PA1");
      assert(t0.isSet("ZDM")==false);
 
      assert(t0.erase("AAG")==true);
      assert(t0.m_First->m_Key == "PA1"
             && t0.m_First->m_NextOrder->m_Key == "UOS"
             && t0.m_First->m_NextOrder->m_NextOrder->m_Key == "PA2"
             && t0.m_First->m_NextOrder->m_NextOrder->m_NextOrder->m_Key == "CAO"
             && t0.m_First->m_NextOrder->m_NextOrder->m_NextOrder->m_NextOrder->m_Key == "LIN"
             && t0.m_First->m_NextOrder->m_NextOrder->m_NextOrder->m_NextOrder->m_NextOrder->m_Key == "AG1");
      assert(t0.m_Last->m_Key == "AG1"
             && t0.m_Last->m_PrevOrder->m_Key == "LIN"
             && t0.m_Last->m_PrevOrder->m_PrevOrder->m_Key == "CAO"
             && t0.m_Last->m_PrevOrder->m_PrevOrder->m_PrevOrder->m_Key == "PA2"
             && t0.m_Last->m_PrevOrder->m_PrevOrder->m_PrevOrder->m_PrevOrder->m_Key == "UOS"
             && t0.m_Last->m_PrevOrder->m_PrevOrder->m_PrevOrder->m_PrevOrder->m_PrevOrder->m_Key == "PA1");
      assert(t0.isSet("AAG")==false);
 
      assert(t0.erase("CAO")==true);
      assert(t0.m_First->m_Key == "PA1"
             && t0.m_First->m_NextOrder->m_Key == "UOS"
             && t0.m_First->m_NextOrder->m_NextOrder->m_Key == "PA2"
             && t0.m_First->m_NextOrder->m_NextOrder->m_NextOrder->m_Key == "LIN"
             && t0.m_First->m_NextOrder->m_NextOrder->m_NextOrder->m_NextOrder->m_Key == "AG1");
      assert(t0.m_Last->m_Key == "AG1"
             && t0.m_Last->m_PrevOrder->m_Key == "LIN"
             && t0.m_Last->m_PrevOrder->m_PrevOrder->m_Key == "PA2"
             && t0.m_Last->m_PrevOrder->m_PrevOrder->m_PrevOrder->m_Key == "UOS"
             && t0.m_Last->m_PrevOrder->m_PrevOrder->m_PrevOrder->m_PrevOrder->m_Key == "PA1");
      assert(t0.isSet("CAO")==false);
 
      assert(t0.erase("UOS")==true);
      assert(t0.m_First->m_Key == "PA1"
             && t0.m_First->m_NextOrder->m_Key == "PA2"
             && t0.m_First->m_NextOrder->m_NextOrder->m_Key == "LIN"
             && t0.m_First->m_NextOrder->m_NextOrder->m_NextOrder->m_Key == "AG1");
      assert(t0.m_Last->m_Key == "AG1"
             && t0.m_Last->m_PrevOrder->m_Key == "LIN"
             && t0.m_Last->m_PrevOrder->m_PrevOrder->m_Key == "PA2"
             && t0.m_Last->m_PrevOrder->m_PrevOrder->m_PrevOrder->m_Key == "PA1");
      assert(t0.isSet("UOS")==false);
 
      assert(t0.erase("UOS")==false);
      assert(t0.m_First->m_Key == "PA1"
             && t0.m_First->m_NextOrder->m_Key == "PA2"
             && t0.m_First->m_NextOrder->m_NextOrder->m_Key == "LIN"
             && t0.m_First->m_NextOrder->m_NextOrder->m_NextOrder->m_Key == "AG1");
      assert(t0.m_Last->m_Key == "AG1"
             && t0.m_Last->m_PrevOrder->m_Key == "LIN"
             && t0.m_Last->m_PrevOrder->m_PrevOrder->m_Key == "PA2"
             && t0.m_Last->m_PrevOrder->m_PrevOrder->m_PrevOrder->m_Key == "PA1");
      assert(t0.isSet("UOS")==false);
 
      assert(t0.erase("LIN")==true);
      assert(t0.m_First->m_Key == "PA1"
             && t0.m_First->m_NextOrder->m_Key == "PA2"
             && t0.m_First->m_NextOrder->m_NextOrder->m_Key == "AG1");
      assert(t0.m_Last->m_Key == "AG1"
             && t0.m_Last->m_PrevOrder->m_Key == "PA2"
             && t0.m_Last->m_PrevOrder->m_PrevOrder->m_Key == "PA1");
      assert(t0.isSet("LIN")==false);
 
      assert(t0.erase("PA1")==true);
      assert(t0.m_First->m_Key == "PA2"
             && t0.m_First->m_NextOrder->m_Key == "AG1");
      assert(t0.m_Last->m_Key == "AG1"
             && t0.m_Last->m_PrevOrder->m_Key == "PA2");
      assert(t0.isSet("PA1")==false);
 
      assert(t0.erase("PA2")==true);
      assert(t0.m_First->m_Key == "AG1");
      assert(t0.m_Last->m_Key == "AG1");
      assert(t0.isSet("PA2")==false);
 
      assert(t0.erase("AG1")==true);
      assert(t0.m_First == t0.m_Last);
      assert(t0.isSet("AG1")==false);
    }
};
 
int main ( )
{
  CTester t;
  t.test();

  //cout << "Done" << endl;
 
  return EXIT_SUCCESS;
}