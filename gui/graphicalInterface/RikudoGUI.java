import java.awt.Color;
import java.io.BufferedWriter;
import java.io.File;
import java.nio.file.Files;
import java.nio.file.Path;
import java.nio.file.Paths;
import java.util.ArrayList;
import java.util.HashMap;
import java.util.LinkedList;
import java.util.List;
import java.util.ListIterator;
import java.util.Map;
import java.util.Scanner;
import java.util.concurrent.TimeUnit;

public class RikudoGUI {
	private final int windowSize = 1024;
	private final Color startColor = Color.BLUE;
	private final Color boundaryColor = Color.RED;
	private final String graphFile = "../../graph/graph.txt";
	private final String pathFile = "example.txt"; 
	private Map<Integer, Axial> intToAxial;
	private Map<Axial, Integer> axialToInt;
	private List<List<Integer>> adjList;
	private final static Axial[] neighbors = new Axial[]{
			new Axial(1, 0), new Axial(0, 1), new Axial(-1, 0), new Axial(0, -1),
			new Axial(1, 1), new Axial(-1, -1)};
	private List<Integer> path;
	
	public RikudoGUI() {
		intToAxial = new HashMap<>();
		axialToInt = new HashMap<>();
		adjList = new ArrayList<>();
	}
	
	public void draw() {
		Image2d img = new Image2d(windowSize);
		
		addHexagons(img);
		//img.addText(new Text("chupa bixo", 50, 50));
		
		//LinkedList<Integer> path = new LinkedList<>();
		//path.add(0);
		//path.add(1);
		//path.add(2);
		//path.add(3);
		
		writeGraph();
		
		try {
			Process p = new ProcessBuilder("../../graph/RikudoSolver", graphFile).start();
			TimeUnit.SECONDS.sleep(1);
		}catch(Exception e) {}
		
		readPath();
		printPath(img, path);
		printLabels(img, path);
		
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
	
	private void writeGraph() {
		StringBuilder graphText = new StringBuilder();
		
		graphText.append(adjList.size() + "\n");
		for(int u = 0; u < adjList.size(); u++) {
			for(int v : adjList.get(u)) {
				graphText.append(String.format("%d %d\n", u, v));
			}
		}
		graphText.append(String.format("-1\n"));
		
		graphText.append(String.format("3 1\n"));
		try {
			Files.write(Paths.get(graphFile), graphText.toString().getBytes());
		}
		catch(Exception e) {
			System.out.println("Error writing graph to file " + graphFile);
		}
	}
	
	private void readPath() {
		path = new LinkedList<>();
		try {
			File file = new File(pathFile);
			Scanner sc = new Scanner(file);
			
			while(sc.hasNext()) {
				path.add(sc.nextInt());
			}
			
			sc.close();
		}
		catch(Exception e) {
			System.out.println("Error reading path from file " + pathFile);
		}
	}
	
	private void printPath(Image2d img, List<Integer> path) {
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
	
	private void printLabels(Image2d img, List<Integer> vertices) {
		ListIterator<Integer> it = path.listIterator();
		while(it.hasNext()) {
			int vertex = it.next();
			Offset offset = intToAxial.get(vertex).toOffset(100);
			img.addText(new Text(Integer.toString(vertex), (float) offset.x, (float) offset.y));
		}
	}
}

