#include <stdexcept>
#include <cmath>

#ifndef MAIN
#include "common.cc"
#endif

/**
 * セグメント木
 *
 * @tparam V1 もとのコンテナの要素の型
 * @tparam V2 Segment木が扱う型
 * @tparam Accumulate V1からV2への変換関数。(V2, V2) -> V2と(V1) -> V2をオーバーロードしている必要がある。
 */
template <typename V1, typename V2, typename Accumulate>
struct SegmentTree {
    template <typename Iterator>
    SegmentTree(const Iterator &begin, const Iterator &end) : m_acc(Accumulate()), m_size(end - begin) {
        size_t n = 1;
        while (n < this->size()) {
            n *= 2;
        }
        this->m_tree.resize(2 * n - 1);

        this->initialize(begin, 0, 0, n);
    }
    template <typename Iterator>
    SegmentTree(const Iterator &begin, const Iterator &end, const Accumulate &acc) : m_acc(acc), m_size(end - begin) {
        size_t n = 1;
        while (n < this->size()) {
            n *= 2;
        }
        this->m_tree.resize(2 * n - 1);

        this->initialize(begin, 0, 0, n);
    }
    SegmentTree(const Vector<V1> &elems) : m_acc(Accumulate()), m_size(elems.size()) {
        size_t n = 1;
        while (n < this->size()) {
            n *= 2;
        }
        this->m_tree.resize(2 * n - 1);

        this->initialize(elems.begin(), 0, 0, n);
    }
    SegmentTree(const Vector<V1> &elems, const Accumulate &acc) : m_acc(acc), m_size(elems.size()) {
        size_t n = 1;
        while (n < this->size()) {
            n *= 2;
        }
        this->m_tree.resize(2 * n - 1);

        this->initialize(elems.begin(), 0, 0, n);
    }

    /**
     * [begin, end)に対応する値をreturnする。
     *
     * @param begin queryをかける先頭の要素
     * @param end queryをかける最後の要素+1
     * @return Accumulate(xs[begin], xs[begin+1], ... xs[end-1])
     * @throw out_of_range [begin, end)が領域外の場合、out_of_rangeをthrowする
     */
    V2 query(size_t begin, size_t end) const {
        auto v = this->query(begin, end, 0, 0, (this->m_tree.size() + 1) / 2);
        if (v) {
            return v.value();
        } else {
            throw std::out_of_range("");
        }
    }

    /**
     * indexの値をvalueに変更する
     *
     * @param index 変更する要素のindex
     * @param value 変更後の値
     */
    void update(size_t index, const V1 &value) {
        // 葉を表す要素へ
        auto k = index + (this->m_tree.size() + 1) / 2 - 1;
        this->m_tree[k] = this->m_acc(value);
        while (k > 0) {
            k = (k - 1) / 2;
            this->m_tree[k] = this->m_acc(value);

            size_t chl = k * 2 + 1;
            size_t chr = k * 2 + 2;
            if (this->m_tree[chl] && this->m_tree[chr]) {
                // 子ノードが両方valid
                this->m_tree[k] = this->m_acc(this->m_tree[chl].value(), this->m_tree[chr].value());
            } else if (this->m_tree[chl]) {
                // 右側は範囲外
                this->m_tree[k] = this->m_tree[chl];
            } else {
                // 両方範囲外
                this->m_tree[k] = {};
            }
        }
    }

    size_t size() const {
        return this->m_size;
    }
private:
    /**
     * m_tree[index]をbegin,...で初期化する。
     * l, rは計算簡略化のために引数にしている。
     *
     * @tparam Iterator Iteratorの型
     * @param begin 保存する要素の先頭
     * @param k 更新するindex
     * @param l 補助変数 (indexに対応するrangeの先頭)
     * @param r 補助変数 (indexに対応するrangeの末尾+1)
     */
    template <typename Iterator>
    void initialize(const Iterator &begin, size_t k, size_t l, size_t r) {
        if (r - l == 1) {
            // leafの場合
            if (l < this->size()) {
                this->m_tree[k] = make_optional(this->m_acc(*(begin + l)));
            } else {
                this->m_tree[k] = {};
            }
        } else {
            // nodeの場合
            size_t chl = k * 2 + 1;
            size_t chr = k * 2 + 2;
            this->initialize(begin, chl, l, (l + r) / 2);
            this->initialize(begin, chr, (l + r) / 2, r);
            if (this->m_tree[chl] && this->m_tree[chr]) {
                // 子ノードが両方valid
                this->m_tree[k] = this->m_acc(this->m_tree[chl].value(), this->m_tree[chr].value());
            } else if (this->m_tree[chl]) {
                // 右側は範囲外
                this->m_tree[k] = this->m_tree[chl];
            } else {
                // 両方範囲外
                this->m_tree[k] = {};
            }
        }
    }
    /**
     * [begin, end)に対応する値をreturnする。
     * index, l, rは計算簡略化のための補助変数
     *
     * @param begin queryの先頭
     * @param end queryの末尾+1
     * @param k 着目しているセグメント木の要素
     * @param l indexが扱うrangeの先頭
     * @param r　indexが扱うrangeの末尾+1
     * @return Accumulate(xs[begin], xs[begin+1], ... xs[end-1])
     */
    std::experimental::optional<V2> query(size_t begin, size_t end, size_t k, size_t l, size_t r) const {
        if (r <= begin || end <= l) {
            // query対象と扱うrangeが交差しない
            return {};
        }

        if (begin <= l && r <= end) {
            // query対象に完全に含まれる
            return this->m_tree[k];
        } else {
            auto vl = this->query(begin, end, k * 2 + 1, l, (l + r) / 2);
            auto vr = this->query(begin, end, k * 2 + 2, (l + r) / 2, r);
            if (vl && vr) {
                // 子ノードが両方valid
                return this->m_acc(vl.value(), vr.value());
            } else if (vl) {
                // 右側は範囲外
                return vl.value();
            } else if (vr) {
                // 左側は範囲外
                return vr.value();
            } else {
                // 両方範囲外
                return {};
            }
        }
    }

    Accumulate m_acc;
    Vector<std::experimental::optional<V2>> m_tree;
    size_t m_size;
};

namespace internal {
    template <typename V>
    struct Minimum {
        V operator()(const V &v) const { return v; }
        V operator()(const V &l, const V &r) const { return std::min(l, r); }
    };
}

template <typename V>
using RangeMinimumQuery = SegmentTree<V, V, internal::Minimum<V>>;
