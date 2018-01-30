public class Diamond {
	// first and second are two vertices that must be adjacent in the hamiltonian path
	public int first, second;
	
	public Diamond(int first, int second) {
		this.first = first;
		this.second = second;
	}
	
	@Override
	public boolean equals(Object obj) {
		if(this == obj) return true;
		if(!(obj instanceof Diamond))	return false;
		Diamond other = (Diamond) obj;
		return first == other.first && second == other.second;
	}
	
	@Override
	public int hashCode() {
		int result = first;
		result = 31 * result + second;
		return result;
	}
}
