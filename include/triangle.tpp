#include "triangle.hpp"

namespace geometry {
	
	template<typename T>
	Triangle_<T>::Triangle_(T a, T b, T c) : m_vertices{a, b, c} {}
	
	template<typename T>
	T const &Triangle_<T>::a() const { return m_vertices[Index::A]; }
	
	template<typename T>
	T const &Triangle_<T>::b() const { return m_vertices[Index::B]; }
	
	template<typename T>
	T const &Triangle_<T>::c() const { return m_vertices[Index::C]; }
	
	template<typename T>
	T const &Triangle_<T>::operator[](int index) const {
		return m_vertices[index];
	}
	
	template<typename T>
	T &Triangle_<T>::a() { return m_vertices[Index::A]; };
	
	template<typename T>
	T &Triangle_<T>::b() { return m_vertices[Index::B]; };
	
	template<typename T>
	T &Triangle_<T>::c() { return m_vertices[Index::C]; };
	
	template<typename T>
	T &Triangle_<T>::operator[](int index) { return m_vertices[index]; }
	
} // namespace geometry
