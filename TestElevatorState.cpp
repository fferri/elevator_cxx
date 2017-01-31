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
    std::set<int> lights = {3, 5};
    S::Ptr s0 = std::make_shared<S>(at, lights);

    // shorthands for golog constructs:
    auto empty = []() -> P::Ptr {return std::make_shared<Empty<S>>();};
    auto exec = [](Action<S> a) -> P::Ptr {return std::make_shared<Exec<S>>(a);};
    auto test = [](Predicate<S> p) -> P::Ptr {return std::make_shared<Test<S>>(p);};
    auto seq = [](P::Ptr p1, P::Ptr p2) -> P::Ptr {return std::make_shared<Sequence<S>>(p1, p2);};
    auto choose = [](P::Ptr p1, P::Ptr p2) -> P::Ptr {return std::make_shared<Choose<S>>(p1, p2);};
    auto star = [](P::Ptr p) -> P::Ptr {return std::make_shared<Star<S>>(p);};
    
    P::Ptr down = exec([](S::Ptr s) -> S::Ptr {return s->down();});
    P::Ptr up = exec([](S::Ptr s) -> S::Ptr {return s->up();});
    P::Ptr turnOff = exec([](S::Ptr s) -> S::Ptr {return s->turnOff();});
    P::Ptr allOff = test([](S::Ptr s) -> bool {return s->lights.empty();});
    
    P::Ptr serve_a_floor = choose(
        seq(star(up), turnOff),
        seq(star(down), turnOff)
    );
    
    P::Ptr serve_all_floors = seq(
        star(serve_a_floor),
        allOff
    );
    
    // give each 'Exec' program a name, so that we can print the plan later
    map<P::Ptr, string> actionName;
    actionName[up] = "up";
    actionName[down] = "down";
    actionName[turnOff] = "turnOff";

    const int maxSolutions = 10;
    int numSolutions = 0;
    ProgramStateIterator<S> it(serve_all_floors, s0);
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

