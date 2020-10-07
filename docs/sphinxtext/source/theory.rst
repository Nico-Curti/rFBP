Theory
-------

The `rFBP` algorithm derives from an out-of-equilibrium (non-Boltzmann) model of the learning process of binary neural networks DallAsta101103_.
This model mimics a spin glass system whose realizations are equally likely to occur when sharing the same so-called entropy (not the same energy, i.e. out-of-equilibrium).
This entropy basically counts the number of solutions (zero-energy realizations) around a realization below a fixed-distance.

Within this out-of-equilibrium framework, the objective is to maximize the entropy instead of minimizing the energy.
From a machine learning standpoint, we aim at those weights sets that perfectly solve the learning process (zero-errors) and that are mathematically closed to each other.
To this end, the Belief Propagation method MézardMontanari_ can be adopted as the underlying learning rule, although it must be properly adjusted to take into account the out-of-equilibrium nature of the model.

The *Replicated Focusing Belief Propagation* (`rFBP`) is an entropy-maximization based algorithm operating as the underlying learning rule of feed-forward binary neural networks.
Here, the entropy is defined as:

:math:`S(\vec{w},\beta,\gamma) = \frac{1}{N} log \bigg ( \sum_{\vec{w}'}  e^{-\beta E(\vec{w}')} e^{\gamma \vec{w}' \cdot \vec{w}}	\bigg)`

where :math:`\vec{w}` is the whole weights set, N is its size and :math:`\beta` is the standard Boltzmann term.
Further, :math:`E(\vec{w})` is the energy of such weights set, which is equal to the number of wrong predictions on the training set produced by :math:`\vec{w}`.
When :math:`\beta \to inf`, only those :math:`\vec{w}` with null energy, i.e. perfect solutions, sum up in the entropy.
At the time being, the rFBP only works with :math:`\beta \to inf`.

The realization of the rFBP is equivalent to evolve a spin-glass system composed of several interacting replicas with the Belief Propagation algorithm. The density distribution of the system is modelled by:

:math:`P(\vec{w} | \beta,y,\gamma) = \frac{e^{yN S(\vec{w},\beta,\gamma)}}{Z(\beta,y,\gamma)}`

where Z is the partition function.

Such spin-glass model depends necessarily on two parameters: y and :math:`\gamma`.
The former is a temperature-alike related variable, similar to the one usually exploited by Gradient Descend approaches, but it can be also interpreted as the number of interacting replicas of the system.
The latter is the penalization term associated to the distance between two weights sets. Indeed, the term :math:`e^{\gamma \vec{w}' \cdot \vec{w}}` in the entropy is larger, when :math:`\vec{w}` and :math:`\vec{w}` are closer.

The Belief Propagation algorithm needs to be to adjusted by adding incoming extra messages for all weights, in order to involve the interacting replicas of the system.
This extra term is represented by:

:math:`\hat{m}^{t_1}_{\star \to w_i} = tanh \big[ (y-1) artanh ( m^{t_0}_{w_i \to \star} tanh \gamma ) \big] tanh \gamma`

where :math:`w_i` and :math:`\star` stand respectively for the i-th weight and a representation of all i-th replicas.

The `rFBP` is therefore an adjusted Belief Propagation algorithm, whose general procedure can be summarized as follows:

- set :math:`\beta \to inf`
- select protocols for y and :math:`\gamma`
- set first values of y and :math:`\gamma` and run the adjusted-BP method until convergence (:math:`\epsilon` or up to a limited-number of iterations;
- step to the next pair values of y and :math:`\gamma` with respect to the chosen protocols and re-run the adjusted-BP method;
- keep it going until a solution is reached or protocols end.

The `rFBP` algorithm focuses the replicated system to fall step by step into weights sets extremely closed to many perfect solutions (:math:`\vec{w}` such that :math:`E(\vec{w})=0`), which ables them to well generalize out of the training set `Zecchina et al`_.

.. _`Zecchina et al`: https://www.pnas.org/content/113/48/E7655
.. _MézardMontanari: https://web.stanford.edu/~montanar/RESEARCH/book.html
.. _DallAsta101103 : https://journals.aps.org/pre/abstract/10.1103/PhysRevE.77.031118
