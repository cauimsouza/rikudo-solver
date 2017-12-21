import java.awt.Color;
import java.util.ArrayList;
import java.util.HashMap;
import java.util.LinkedList;
import java.util.ListIterator;

public class RikudoGUI {
	private final int windowSize = 1024;
	private final Color startColor = Color.BLUE;
	private final Color boundaryColor = Color.RED;
	private HashMap<Integer, Axial> intToAxial;
	private HashMap<Axial, Integer> axialToInt;
	private ArrayList<LinkedList<Integer>> adjList;
	private final static Axial[] neighbors = new Axial[]{
			new Axial(1, 0), new Axial(0, 1), new Axial(-1, 0), new Axial(0, -1),
			new Axial(1, 1), new Axial(-1, -1)}; 
	
	public RikudoGUI() {
		intToAxial = new HashMap<>();
		axialToInt = new HashMap<>();
		adjList = new ArrayList<>();
	}
	
	public void draw() {
		Image2d img = new Image2d(windowSize);
		
		addHexagons(img);
		
		LinkedList<Integer> path = new LinkedList<>();
		path.add(0);
		path.add(1);
		path.add(2);
		path.add(3);
		printPath(img, path);
		
		new Image2dViewer(img);	
	}
	
	private void addHexagons(Image2d img){
		int refx = 3;
		int refy = 3;
		
		addHexagon(img, refx, refy);
		addHexagon(img, refx, refy-1);
		addHexagon(img, refx-1, refy);
		addHexagon(img, refx-1, refy+1);
	}
	
	private void addHexagon(Image2d img, int x, int y) {
		Axial axial = new Axial(x, y);
		int n = intToAxial.size();
		
		intToAxial.put(n, axial);
		axialToInt.put(axial, n);
		adjList.add(new LinkedList<>());
		img.addHexagon(axial, startColor, boundaryColor);
		
		for(int i = 0; i < neighbors.length; i++) {
			Axial neighbor = neighbors[i].add(axial);
			if(axialToInt.containsKey(neighbor)) {
				int neighborVertex = axialToInt.get(neighbor);
				adjList.get(n).add(neighborVertex);
				adjList.get(neighborVertex).add(n);
			}
		}
	}
	
	private void printPath(Image2d img, LinkedList<Integer> path) {
		ListIterator<Integer> it = path.listIterator();
		int prev = it.next();
		while(it.hasNext()) {
			int cur = it.next();
			Offset prevOffset = intToAxial.get(prev).toOffset(100);
			Offset curOffset = intToAxial.get(cur).toOffset(100);
			
			img.addSegment(prevOffset.x, prevOffset.y, curOffset.x, curOffset.y, 10, Color.BLACK);
			
			prev = cur;
		}
	}
}

