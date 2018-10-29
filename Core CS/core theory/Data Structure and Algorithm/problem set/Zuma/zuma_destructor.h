#pragma once

template <typename T> Zuma<T>::~Zuma() {
	clear();
	delete header; delete trailer;
}