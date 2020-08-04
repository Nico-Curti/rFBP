# Replicated Focusing Belief Propagation

The *Replicated Focusing Belief Propagation* (`rFBP`) is an entropy-maximization based algorithm operating as the underlying learning rule of feed-forward binary neural networks.
Here, the entropy is defined as:

<img src="https://render.githubusercontent.com/render/math?math={S(\vec{w},\beta,\gamma) = \frac{1}{N} log \bigg ( \sum_{\vec{w}'}  e^{-\beta E(\vec{w}')} e^{\gamma \vec{w}' \cdot \vec{w}}	\bigg)}"> ,

where <img src="https://render.githubusercontent.com/render/math?math={\vec{w}}"> is the whole weights set, N is its size and <img src="https://render.githubusercontent.com/render/math?math={\beta}"> is the standard Boltzmann term.
Further, <img src="https://render.githubusercontent.com/render/math?math={E(\vec{w})}"> is the energy of such weights set, which is equal to the number of wrong predictions on the training set produced by <img src="https://render.githubusercontent.com/render/math?math={\vec{w}}">.
When <img src="https://render.githubusercontent.com/render/math?math={\beta \to inf}">, only those <img src="https://render.githubusercontent.com/render/math?math={\vec{w}'}"> with null energy, i.e. perfect solutions, sum up in the entropy.
At the time being, the rFBP only works with <img src="https://render.githubusercontent.com/render/math?math={\beta \to inf}">.

The realization of the rFBP is equivalent to evolve a spin-glass system composed of several interacting replicas with the Belief Propagation algorithm. The density distribution of the system is modelled by:

<img src="https://render.githubusercontent.com/render/math?math={P(\vec{w} | \beta,y,\gamma) = \frac{e^{yN S(\vec{w},\beta,\gamma)}}{Z(\beta,y,\gamma)}}"> ,

where Z is the partition function.

Such spin-glass model depends necessarily on two parameters: y and <img src="https://render.githubusercontent.com/render/math?math={\gamma}">.
The former is a temperature-alike related variable, similar to the one usually exploited by Gradient Descend approaches, but it can be also interpreted as the number of interacting replicas of the system.
The latter is the penalization term associated to the distance between two weights sets. Indeed, the term <img src="https://render.githubusercontent.com/render/math?math={e^{\gamma \vec{w}' \cdot \vec{w}}}"> in the entropy is larger, when <img src="https://render.githubusercontent.com/render/math?math={\vec{w}}"> and <img src="https://render.githubusercontent.com/render/math?math={\vec{w}'}"> are closer.

The Belief Propagation algorithm needs to be to adjusted by adding incoming extra messages for all weights, in order to involve the interacting replicas of the system.
This extra term is represented by:

<img src="https://render.githubusercontent.com/render/math?math={\hat{m}^{t_1}_{\star \to \w_i} = tanh \big[ (y-1) artanh ( m^{t_0}_{\w_i \to \star} tanh \gamma ) \big] tanh \gamma}">,

where <img src="https://render.githubusercontent.com/render/math?math={\w_i}"> and <img src="https://render.githubusercontent.com/render/math?math={\star}"> stand respectively for the i-th weight and a representation of all i-th replicas.

The `rFBP` is therefore an adjusted Belief Propagation algorithm, whose general procedure can be summarized as follows:
- set <img src="https://render.githubusercontent.com/render/math?math={\beta \to inf}">
- select protocols for y and <img src="https://render.githubusercontent.com/render/math?math={\gamma}">;
- set first values of y and <img src="https://render.githubusercontent.com/render/math?math={\gamma}"> and run the adjusted-BP method until convergence (<img src="https://render.githubusercontent.com/render/math?math={ < \epsilon}">) or up to a limited-number of iterations;
- step to the next pair values of y and <img src="https://render.githubusercontent.com/render/math?math={\gamma}"> with respect to the chosen protocols and re-run the adjusted-BP method;
- keep it going until a solution is reached or protocols end.

The `rFBP` algorithm focuses the replicated system to fall step by step into weights sets extremely closed to many perfect solutions (<img src="https://render.githubusercontent.com/render/math?math={\vec{w}}"> such that <img src="https://render.githubusercontent.com/render/math?math={E(\vec{w})=0}">), which ables them to well generalize out of the training set [[Baldassi101103](https://journals.aps.org/prl/pdf/10.1103/PhysRevLett.115.128101)].
