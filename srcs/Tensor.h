// Copyright Aaryn Tonita, 2011
// Distributed under the Gnu general public license
#ifndef TENSOR_H_
#define TENSOR_H_
#include <cstdarg>

/**
 * \brief The class Tensor encapsulates the components of a tensor at a
 * given point (not the entire manifold) and also the logic necessary to
 * perform algebra with Tensors. 
 *
 * The dimension is assumed to be 4.
 * Indexing of components and indices begin at zero. So for Z^a_b, a is
 * the 0th index and runs from 0-3.
 */
class Tensor {
  public:
    /**
     * Named types for the indices. There are synonyms: UP are
     * CONTRAVARIANT (vector type) while DOWN are COVARIANT indices
     * (covector type).
     */
    enum IndexType {
      COVARIANT = -1,   /**< Specifies a covariant tensor index */
      CONTRAVARIANT = 1,/**< Specifies a contravariant tensor index */
      DOWN = -1,        /**< Specifies a covariant tensor index */
      UP = 1            /**< Specifies a contravariant tensor index */
    };

    /**
     * \brief Default constructor.
     *
     * Creates the point tensor. Sets the rank, creates storage, zeros
     * the components and sets the contravariant/covariant type of the
     * indices.
     * \param Rank The rank of the tensor.
     * \param Types The types of the indices.
     */
    Tensor(int Rank, IndexType* Types);

    /**
     * Copy constructor.
     * \param original The original Tensor, to copy.
     */
    Tensor(const Tensor &original);

    /**
     * \brief Gets a component.
     *
     * \param indices The indices of the desired component starting at
     * 0.
     * \param value The value of the component to set.
     * \retval component The selected component.
     */
    void setComponent(int* indices, double value);

    /**
     * \brief Gets a component.
     *
     * \param indices The indices of the desired component starting at
     * 0.
     * \retval component The selected component.
     */
    double getComponent(int* indices) const;

    /**
     * \brief Returns a pointer to the components.
     *
     * Since one can individually change the components via the index,
     * this routine is provided so one can get all the data for
     * printing, editing, evolving or whatnot.
     * \retval components A pointer to the array storing the components.
     */
    double* getComponents() const;

    /**
     * \brief An indexing function. 
     *
     * To abstract away the storage model. Converts n=rank indices into a
     * single 1-d index. This is used to get the actual component from
     * the 1d storage array.
     */
    int index(int* indices) const;

    /**
     * \brief Converts 1d index to rank-d.
     *
     * Converts the 1d index (as returned from index()) into an
     * array of indices from 0-DIMENSION-1.
     * \param index The 1d index.
     * \param indices The indices array to set.
     */
    void indexToIndices(int index, int* indices) const;

    /**
     * \brief Returns the rank of this tensor.
     *
     * \retval rank The rank of this tensor.
     */
    int getRank() const;

    /**
     * \brief Returns a const pointer to the index types of this Tensor.
     *
     * \retval types The index types of this tensor.
     */
    const IndexType* getTypes() const;

    /**
     * \brief Contracts this tensor over chosen indices.
     *
     * Contract tensor index1 with tensor index2. Computes a trace.
     * That is result = A^[i1..index1..in]_[j1..index2..jm]
     * \param index1 The first index to contract with.
     * \param index2 The second index to contract with.
     * \retval result The tensor to store the result in.
     */
    Tensor contract(int index1, int index2) const;

    /**
     * \brief Names the indices.
     *
     * Indexing operation. Sets the abstract indexes or nulls them, for
     * safety. If an index is 0, '.' or '-', it will not be summed over.
     * Otherwise, identical indexes are contracted. If one has delta^a_b
     * and then calls delta(a,a), then contraction will be performed.
     *
     * Initial values are 0.
     * \param i1 The first index or NULL, to remove all indices.
     * \param ... The va_list of other indexes.
     * \retval this The indexed tensor, possibly with indexes contracted.
     */
    Tensor operator()(char i1, ...);

    /**
     * \brief Destructor.
     */
    virtual ~Tensor();

    /**
     * \brief Scalar inplace multiplication.
     *
     * \param scalar The scalar to multiply by.
     * \retval this A reference to this tensor.
     */
    Tensor & operator*=(const double scalar);

    /**
     * \brief Inplace addition.
     *
     * \param tensor The tensor to add to this tensor.
     * \retval this A reference to this tensor after the addtion.
     */
    Tensor & operator+=(const Tensor &tensor);

    /**
     * \brief Inplace subtraction.
     *
     * \param tensor The tensor to subtract from this tensor.
     * \retval this A reference to this tensor after the subtraction.
     */
    Tensor & operator-=(const Tensor &tensor) {
      (*this) += (-1)*tensor;
      return *this;
    }

    /**
     * \brief Scalar multiplication.
     *
     * \param scalar The scalar to multiply by.
     * \retval A new tensor which is equal to this one multiplied by
     * scalar.
     */
    Tensor operator*(const double scalar) const;

    /**
     * \brief Tensor multiplication.
     *
     * After multiplication, contractions are performed (if any).
     * \param tensor The tensor to multiply by.
     * \retval A new tensor which is equal to this one multiplied by
     * the supplied tensor.
     */
    Tensor operator*(const Tensor& tensor) const;

    /**
     * \brief Addition.
     * \param tensor The tensor to add to this one.
     * \retval A new tensor equal to this one with the argument added to
     * it.
     */
    Tensor operator+(const Tensor &tensor) const;

    /**
     * \brief Subtraction.
     * \param tensor The tensor to subtract from this one.
     * \retval A new tensor equal to this one with the argument
     * subtracted.
     */
    Tensor operator-(const Tensor &tensor) const {
      return operator+((-1)*tensor);
    };

    /**
     * \brief Scalar multiplication is commutative.
     * \param scalar The scalar to multiply by.
     * \param tensor The tensor to be multiplied.
     * \retval result The tensor multiplied by the scalar.
     */
    friend Tensor operator*(const double scalar, 
        const Tensor &tensor) { return tensor*scalar;};

  protected:

    /**
     * \brief Contracts.
     *
     * If there are indices which should be contracted, it contracts
     * them and returns the result.
     */
    Tensor contract() const;

    /**
     * \brief The indices, for summation.
     */
    char* indexes;

    /**
     * \brief The components of the tensor.
     */
    double* components;

    /**
     * \brief Constructor which names indices.
     *
     * Creates the point tensor. Sets the rank, creates storage, zeros
     * the components and sets the contravariant/covariant type of the
     * indices.
     * \param Rank The rank of the tensor.
     * \param Types The types of the indices.
     * \param Indexes The indexes used for the tensor.
     */
    Tensor(int Rank, IndexType* Types, char* Indexes);

  private:
    /**
     * \brief The rank of the tensor.
     */
    int rank;

    /**
     * \brief The types of the tensor indexes.
     */
    IndexType* types;

    /**
     * \brief Integer power function.
     *
     * Used because one sums from i = 0 .. D^rank.
     * \param i The base of the power.
     * \param j The exponent.
     * \retval k = i^j
     */
    int ipow(int i, int j) const;

    /**
     * \brief Allocates storage.
     *
     * Used by the constructors to allocate memory.
     * \param Rank The rank of the tensor.
     * \param Types The types of the indices.
     */
    void init(int Rank, IndexType* Types);
};

#endif