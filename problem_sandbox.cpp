// In this tutorial we learn how to implement gradients and hessian
// in case whe have them. The problem is to minimize:
// f = x1^2 + x2^2 + x3^2 + x4^2 in the bounds:
// -10 <= xi <= 10
//

// All we need to do is to implement a struct (or class) having the
// following mandatory methods: 
//
// fitness_vector fitness(const decision_vector &)
// decision_vector::size_type get_n() const
// fitness_vector::size_type get_nf() const
// std::pair<decision_vector, decision_vector> get_bounds() const
//
// And add a method:
// gradient_vector gradient(const decision_vector &x)

#include <string>

#include "include/io.hpp"
#include "include/problem.hpp"
#include "include/types.hpp"



using namespace pagmo;
struct example0_g
{
    // Mandatory, computes ... well ... the fitness
    fitness_vector fitness(const decision_vector &x) const
    {
        fitness_vector retval(1);
        retval[0] = x[0]*x[0] + x[1]*x[1] + x[2]*x[2] + x[3]*x[3];
        return retval;
    }

    // Optional, computes the gradient. In this simple case
    // df/dx0, df/dx1, df/dx2, df/dx3
    gradient_vector gradient(const decision_vector &x) const
    {
        gradient_vector retval(4,0.);
        retval[0] = 2 * x[0];
        retval[1] = 2 * x[1];
        retval[2] = 2 * x[2];
        retval[3] = 2 * x[3];
        return retval;
    }

    sparsity_pattern dsparsity() const
    {
        sparsity_pattern retval;
        for (auto i=0u; i<4u; ++i) {
            retval.push_back(std::pair<long, long>(0, i));
        }
        return retval;
    }

    // Mandatory, returns the dimension of the decision vector,
    // in this case fixed to 4
    decision_vector::size_type get_n() const
    {
        return 4u;
    }

    // Mandatory, returns the dimension of the decision vector,
    // in this case fixed to 1 (single objective)
    fitness_vector::size_type get_nf() const
    {
        return 1u;
    }
    
    // Mandatory, returns the box-bounds
    std::pair<decision_vector, decision_vector> get_bounds() const
    {
        decision_vector lb{-10,-10,-10,-10};
        decision_vector ub{ 10, 10, 10, 10};
        return std::pair<decision_vector, decision_vector>(std::move(lb), std::move(ub));
    }

    // Optional, provides a name for the problem overrding the default name
    std::string get_name() const
    {   
        return std::string("My Problem");
    }

    // Optional, provides extra information that will be appended after
    // the default stream operator
    std::string extra_info() const {
        std::ostringstream s;
        s << "This is a simple toy problem with one fitness, " << '\n';
        s << "no constraint and a fixed dimension of 4." << "\n";
        s << "The fitness function gradients are also implemented" << "\n";
        return s.str();
    }
    
    // Optional methods-data can also be accessed later via 
    // the problem::extract() method
    std::vector<decision_vector> best_known() const
    {
        return std::vector<decision_vector>{decision_vector{0,0,0,0}};
    }

};

int main()
{
    // Constructing a problem
    problem p0{example0_g{}};
    // Streaming to screen the problem
    std::cout << p0 << '\n';
    // Getting its dimensions
    std::cout << "Calling the dimension getter: " << p0.get_n() << '\n';
    std::cout << "Calling the fitness dimension getter: " << p0.get_nf() << '\n';

    // Getting the bounds via the pagmo::print eating also std containers
    pagmo::print("Calling the bounds getter: ", p0.get_bounds(), "\n");

    // As soon as a problem its created its function evaluation counter
    // is set to zero. Checking its value is easy
    pagmo::print("fevals: ", p0.get_fevals(), "\n");
    // Computing one fitness
    pagmo::print("calling fitness in x=[2,2,2,2]: ", p0.fitness(decision_vector{2,2,2,2}), "\n");
    // The evaluation counter is now ... well ... 1
    pagmo::print("fevals: ", p0.get_fevals(), "\n");

    // Computing one gradient
    pagmo::print("calling gradient in x=[2,2,2,2]: ", p0.gradient(decision_vector{2,2,2,2}), "\n");

    pagmo::print(p0.sparsity(), "\n");

    // While our example0 struct is now hidden inside the pagmo::problem
    // we can still access its methods / data via the extract interface
    pagmo::print("Accessing best_known: ", p0.extract<example0_g>()->best_known(), "\n");
 
}