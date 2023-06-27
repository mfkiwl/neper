.. _odfsampling:

Random sampling of ODFs
=======================

Given an ODF defined on a mesh, either on elements (piecewise constant) or on nodes (piecewise linear), how can we sample it randomly?

Case of a uniform ODF
---------------------

If the ODF is uniform (no texture), then we can choose the orientations without taking it into account, from random numbers (:math:`n_1`, :math:`n_2`, :math:`n_3`, :math:`n_4 \in [0,\,1]`):

- Euler-Bunge angles: :math:`\varphi_1=2\,\pi\,n_1`, :math:`\varphi_2= \hbox{acos} (2\,n_2-1)`, :math:`\varphi_3=2\,\pi\,n_3`
- unit quaternion: if :math:`{{n_1}^2+{n_2}^2+{n_3}^2+{n_4}^2} \leq 1` then accept orientation :math:`q_i=n_i`
- homochoric vector:  if :math:`\sqrt{{n_1}^2+{n_2}^2+{n_3}^2} \leq 1` then accept orientation :math:`x_i=n_i`

Case of a non-uniform ODF
-------------------------

If the ODF is non-uniform, the frequency of orientations must adhere to the intensity of the ODF (by definition).  One may distinguish random sampling and uniform sampling, however.  Uniform sampling (just as we implemented it for the case of a uniform ODF) attempts to fit the ODF at best given the number of orientations available.  Random sampling (just as we apply it to the case of a uniform ODF) follows the ODF from a statistical point of view, but does not attempt to fit it at best; it retains a random character.  This is the case developed here.

Let the ODF be defined as

.. math::

  \begin{equation}
    f(\mathbf{x}) = \frac{1}{V} \, \frac{\hbox{d}V(\mathbf{x})}{\hbox{d}\mathbf{x}},
  \end{equation}

where :math:`\mathbf{x}` is the orientation.  Let :math:`f_\text{max}` be its maximum.

Here is the procedure:

1. Pick a random orientation, :math:`\mathbf{x}`, following a uniform ODF
2. Generate a random number :math:`t\in[0,\,1]`
3. if :math:`t < f(\mathbf{x}) / f_\text{max}`, then accept orientation

Terminate when all orientations have been generated.
