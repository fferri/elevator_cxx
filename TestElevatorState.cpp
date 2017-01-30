#include "ElevatorState.h"
#include "ProgramStateIterator.h"
#include "Program.h"
#include "Empty.h"
#include "Exec.h"
#include "Test.h"
#include "Sequence.h"
#include "Star.h"
#include "Choose.h"

#include <iostream>
#include <string>
#include <map>
#include <stack>

using std::cout;
using std::endl;
using std::map;
using std::string;
using std::stack;

int main()
{
    using S = ElevatorState;
    using P = Program<S>;

    int at = 0;
    set<int> lights = {3, 5};
    S::Ptr s0 = S::make_shared(at, lights);

    P::Ptr down = exec<S>([](S::Ptr s) -> S::Ptr {return s->down();});
    P::Ptr up = exec<S>([](S::Ptr s) -> S::Ptr {return s->up();});
    P::Ptr turnOff = exec<S>([](S::Ptr s) -> S::Ptr {return s->turnOff();});
    P::Ptr allOff = test<S>([](S::Ptr s) -> bool {return s->lights.empty();});
    
    P::Ptr serve_a_floor = choose<S>(
        seq<S>(star<S>(up), turnOff),
        seq<S>(star<S>(down), turnOff)
    );
    
    P::Ptr serve_all_floors = seq<S>(
        star<S>(serve_a_floor),
        allOff
    );
    
    map<P::Ptr, string> actionName;
    actionName[up] = "up";
    actionName[down] = "down";
    actionName[turnOff] = "turnOff";
    
    P::Ptr p = serve_all_floors;

    const int maxSolutions = 10;
    int numSolutions = 0;
    ProgramStateIterator<S> it(make_shared<ProgramState<S>>(p, s0));
    while(it.hasNext())
    {
        ProgramState<S>::Ptr ps = it.next();
        
        stack<string> plan;
        ProgramState<S>::Ptr tmp = ps;
        while(tmp)
        {
            if(tmp->action) plan.push(actionName[tmp->action]);
            tmp = tmp->parent;
        }
        
        cout << "transStar: s = " << *ps->state << ", plan = ";
        string sep = "";
        while(!plan.empty())
        {
            cout << sep << plan.top();
            plan.pop();
            sep = ", ";
        }
        cout << endl;

        if(++numSolutions >= maxSolutions)
        {
            cout << "Showing only first " << maxSolutions << " solutions" << endl;
            break;
        }
    }
}

