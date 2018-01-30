
public class Point {
	public int x;
	public int y;
	
	public Point(int x, int y) {
		this.x = x;
		this.y = y;
	}
	
	@Override
	public boolean equals(Object obj) {
		if(this == obj) return true;
		if(!(obj instanceof Point))	return false;
		Point other = (Point) obj;
		return x == other.x && y == other.y;
	}
	
	@Override
	public int hashCode() {
		int result = x;
		result = 31 * result + y;
		return result;
	}
}

class Axial extends Point{
	// in this class x represents q (horizontal displacement ->)
	// and y represents r (vertical displacement -> + \/)
	Axial(int x, int y){
		super(x, y);
	}
	
	public Offset toOffset(int size){
		double _y = size * 1.5 * y;
		double _x = size * Math.sqrt(3) * (x + y * 0.5);
		
		return new Offset((int) _x, (int) _y);
	}
	
	public Axial add(Axial a) {
		return new Axial(x + a.x, y + a.y);
	}
}

class Offset extends Point {
	public Offset(int x, int y) {
		super(x, y);
	}
	
	public Axial toAxial(int size) {
		double q = (Math.sqrt(3.0) * x - y) / (3.0 * size);
		double r = (2 * y) / (3.0 * size);
		
		return axialRound(q, r);
	}
	
	private Axial axialRound(double q, double r) {
		double s = -q - r;
		int rq = (int) Math.round(q);
		int rr = (int) Math.round(r);
		int rs = (int) Math.round(s);
		double qdiff = Math.abs(rq - q);
		double rdiff = Math.abs(rr - r);
		double sdiff = Math.abs(rs - s);
		
		if(qdiff > rdiff && qdiff > sdiff)
			rq = -rr - rs;
		else if(rdiff > sdiff)
			rr = -rq - rs;
		else
			rs = -rq - rr;
		
		return new Axial(rq, rr);
	}
}