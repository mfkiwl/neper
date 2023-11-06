.. _ori_statistics:

Specifying Orientation Information for Tessellation Cells
=========================================================

.. important:: Read first: :ref:`simple_model` and (optionally) :ref:`pole_figure`.

Orientations for the cells can be specified as input, using option :option:`-ori`.  Instead of the default "random orientations", specific orientation distributions can be generated.

It is important to note that two concepts are involved in the generation of cell orientations:
  - The *orientation distribution function (ODF)* is the statistical or "frequency" (continuous) distribution of the orientations.
    It is a material attribute.
    An ODF can be *uniform*, if there is no preferential orientation(s), or *non-uniform*, in which case it is typically defined over orientation space.

  - The *distribution discretization (or "sampling") method* represents how discrete orientations are obtained from the ODF.
    It can be *random* or *uniform*.  Given an ODF, *random sampling* "picks" orientations from it according to a random process, so that the discrete orientations match the ODF in a statistical sense, as in a real material.  In contrast, *uniform sampling* generates a set of orientations that fits the ODF as closely as possible (which does not occur in a real material).

Following this terminology, the standard case of "random orientations" corresponds to *random* sampling of a *uniform* orientation distribution function (ODF).  This tutorial discusses specifically the orientation distribution function input, while the distribution discretization method is considered random throughout.

Specifying an orientation distribution allows to go beyond the standard case of "random orientations", which is the default behavior:

.. code-block:: console

  $ neper -T -n 1000 -crysym cubic

This generates a :ref:`tess_file` named :file:`n1000-id1.tess`.

.. note::

  - In general, it is important to define the crystal symmetry, using :option:`-crysym`.
  - It is possible to generate only orientations (no actual tessellation) using :option:`-format` :data:`ori`, which only produces an orientation file (technically a :ref:`data_file`) named :file:`n1000-id1.ori`.
  - In the case where an actual tessellation is generated, the cell orientations are generated after, and fully independently of the tessellation morphology.

The orientations can be visualized on a pole figure using the :ref:`neper_v` and :option:`-space` :data:`pf` (and we recognize random orientations):

.. code-block:: console

  $ neper -V n1000-id1.tess -imagesize 500:500 -space pf -print img1

.. image:: ori_statistics/img1.png

.. _specifying_an_odf:

Specifying an ODF
-----------------

An ODF can be specified from a mesh of orientation space and intensity values.  Of note, a mesh of the fundamental region of Rodrigues space (for the considered crystal symmetry, here *cubic*) can be generated as follows:

.. code-block:: console

  $ neper -T -n 1 -domain "rodrigues(cubic)" -o fr
  $ neper -M fr.tess -cl 0.05

The elemental values of the ODF are defined (externally) on the mesh, and are written in the following file: :download:`odf.txt <ori_statistics/odf.txt>`, which defines a generic rolling texture of an FCC material.  The mesh of orientation space (generated as described above) is provided by the following file: :download:`fr.msh <ori_statistics/fr.msh>`.

Cell orientations can be generated from the ODF as follows:

.. code-block:: console

  $ neper -T -n 1000 -ori "odf(mesh=file(fr.msh),val=file(odf.txt)" -crysym cubic

The orientations can be visualized as before (and we can recognize the rolling texture):

.. code-block:: console

  $ neper -V n1000-id1.tess -imagesize 500:500 -space pf -print img2

.. image:: ori_statistics/img2.png

Using Discrete Orientations and Orientation Fibers
--------------------------------------------------

An orientation distribution can be defined from "base", discrete orientations and orientation fibers (see :option:`-ori` and :ref:`rotations_and_orientations`), optionally surrounded by distributions.

A simple case of Cube-type orientation distribution can be obtained as follows:

.. code-block:: console

  $ neper -T -n 1000 -crysym cubic -ori "Cube:normal(8)"

.. note:: ":data:`normal(8)`" generates a distribution about the nominal orientation corresponding to a tri-variate normal distribution (in tangent space, :math:`r\,\theta`) of 1D standard deviation equal to 8 degrees.  When applied to an orientation fiber, the distribution becomes bi-variate (perpendicular to the fiber).

The orientations can be visualized as before:

.. code-block:: console

  $ neper -V n1000-id1.tess -imagesize 500:500 -space pf -print img3

.. image:: ori_statistics/img3.png

A simple case of a :math:`\left<1 0 0\right> | X_i` fiber surrounded by a distribution of a 1D standard deviation of 4 degrees can be obtained as follows:

.. code-block:: console

  $ neper -T -n 1000 -crysym cubic -ori "fiber(1,0,0,1,0,0):normal(4)"

The orientations can be visualized as before:

.. code-block:: console

  $ neper -V n1000-id1.tess -imagesize 500:500 -space pf -print img5

.. image:: ori_statistics/img5.png

Several orientations (or orientation fibers) can be used to reproduce more complex distributions, each with its own weight and distribution.
An example is as follows:

.. code-block:: console

  $ neper -T -n 1000 -crysym cubic -ori "2*Brass1:normal(3.9)+2*Brass2:normal(3.9)+S1:normal(4.7)+S2:normal(4.7)+S3:normal(4.7)+S4:normal(4.7)+0.8*Copper1:normal(5.6)+0.8*Copper2:normal(5.6)+Cube:normal(3.9)+2*Goss:normal(3.9)"

The orientations can be visualized as before:

.. code-block:: console

  $ neper -V n1000-id1.tess -imagesize 500:500 -space pf -print img4

.. image:: ori_statistics/img4.png

Incidentally, it is similar to the one generated in :ref:`specifying_an_odf`.
