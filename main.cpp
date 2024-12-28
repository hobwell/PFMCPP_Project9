/*
 Project 9: Part 1/1
 Chapter 5 Part 7 Task

 Create a branch named Part1

Purpose:  This project will teach you about variadic templates and recursive templates, one of the last areas needing discussion while learning C++

0) Do your best to avoid looking at previous student submissions for this project.
    This project does not have a lot of code.  
    There are only 1 or 2 ways to solve it.  everyone always arrives at the same solution.
    If you look at the previous student submissions, you will be denying yourself the opportunity to figure this project out for yourself.
    If you get stuck, I would prefer if you message me for help instead of looking at the previous student submissions.
 
1) read Instructions.cpp

Make the following program work, which makes use of Variadic templates and Recursion
 */

#include <iostream>
#include <string>
#include <typeinfo>

struct Point
{
    Point (float _x, float _y) : x (_x), y (_y) {}

    Point& multiply (float m)
    {
        x *= m;
        y *= m;
        return *this;
    }

    std::string toString() const
    {
        std::string str;
        str +="Point { x: ";
        str += std::to_string (x);
        str += ", y: ";
        str += std::to_string (y);
        str += " }";
        return str;
    }

private:
    float x{0}, y{0};
};

template<typename Type>
struct Wrapper
{
    Wrapper (Type&& t) : val (std::move (t)) 
    { 
        std::cout << "Wrapper(" << typeid (val).name() << ")" << std::endl;
    }

    // Step 5
    void print() const
    {
        std::cout << "Wrapper::print(" << val << ")" << std::endl;
    }

    Type val;
};

// Step 8
// Challenge 1
// I specialized the Wrapper class template to work with the Point class by adding the following specialization
template<>
void Wrapper<Point>::print() const
{
   std::cout << "Wrapper<Point>::print(" << val.toString() << ")" << std::endl;   
}
    
/*
 MAKE SURE YOU ARE NOT ON THE MASTER BRANCH

 Commit your changes by clicking on the Source Control panel on the left, entering a message, and click [Commit and push].
 
 If you didn't already: 
    Make a pull request after you make your first commit
    pin the pull request link and this repl.it link to our DM thread in a single message.

 send me a DM to review your pull request when the project is ready for review.

 Wait for my code review.
 */

/*
Challenge 2
I believe the existance of this method ensures that the recursive variadic template
function will exit when the parameter pack is empty 

it must be declared before the variadic template or the project will not compile due 
to an error: 

call to function 'variadicHelper' that is neither visible in the template definition nor found by argument-dependent lookup
*/
void variadicHelper () 
{ 
    std::cout << "Nothing to see here." << std::endl;
}

template<typename T, typename ...Args>
void variadicHelper (T&& first, Args&& ... remainingArgs)
{
    // Step 6 & 9
    Wrapper (std::forward<T> (first)).print();

    // Step 7 & 9
    variadicHelper (std::forward<Args> (remainingArgs) ...);    
}

/* 
Challenge 2:
There is another recursive solution. it is possible for Args ... to be empty.
your task: remove your implementation for instruction 4) and implement this alternate solution.
implement it after your existing variadicHelper function.


hobwell: I had to be define it before the variadicHelper function, see above

hint: what does the recursive call actually look like when called with an empty Variadic Parameter list

hobwell: it looks like a call to a function with no arguments:

#ifdef INSIGHTS_USE_TEMPLATE
template<>
void variadicHelper<>()
{
  std::operator<<(std::cout, "Nothing to see here.").operator<<(std::endl);
}
#endif

hobwell:  my thought was that the solution was to provide an overload that takes no
arguments so that the other variadic function will call it when remainingArgs is empty.

*/
int main()
{
    variadicHelper (3, std::string ("burgers"), 2.5, Point{3.f, 0.14f});
}

/*
hobwell: I have no idea why the warnings are being generated:  
main.cpp:101:5: warning: 'Wrapper' may not intend to support class template argument deduction [-Wctad-maybe-unsupported]

They seem to stem from line 143, where the helper is being called with rvalues, which is what the template is looking for.

If I examine this code on cppinsights, the variadic template appears to unroll as I would expect it to.

Adding the deduction guide below clears the warnings
*/
/*
template <typename T>
Wrapper(T&&) -> Wrapper<T>;
*/