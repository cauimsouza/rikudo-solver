import java.awt.Color;
import java.awt.image.BufferedImage;
import java.io.BufferedWriter;
import java.io.File;
import java.io.IOException;
import java.net.URISyntaxException;
import java.nio.file.Files;
import java.nio.file.Path;
import java.nio.file.Paths;
import java.util.ArrayList;
import java.util.List;
import java.util.Map;
import java.util.HashMap;
import java.util.LinkedList;
import java.util.ListIterator;
import java.util.Scanner;
import java.util.TreeMap;
import java.util.concurrent.TimeUnit;

import javax.imageio.ImageIO;

public class SolverInterface {
	private final int windowSize = 1024;
	private final Color startColor = Color.BLUE;
	private final Color boundaryColor = Color.RED;
	private final String graphFile = "graph.txt";
	private final String solutionFile = "solution.txt"; 
	private final String imageFile = "input.png";
	private HashMap<Integer, Axial> intToAxial;
	private HashMap<Axial, Integer> axialToInt;
	private ArrayList<LinkedList<Integer>> adjList;
	private LinkedList<Integer> path;
	private ArrayList<Diamond> diamonds;
	private TreeMap<Integer, Integer> partialMap;
	private int source, destination;
	
	
	private final static Axial[] neighbors = new Axial[]{
			new Axial(1, 0), new Axial(0, 1), new Axial(-1, 0), new Axial(0, -1),
			new Axial(1, -1), new Axial(-1, 1)};
	
	
	public SolverInterface() {
		intToAxial = new HashMap<>();
		axialToInt = new HashMap<>();
		adjList = new ArrayList<>();
	}
	
	public void draw() {
		BufferedImage file = null;
		try {
			//String path = SolverInterface.class.getProtectionDomain().getCodeSource().getLocation().toURI().getPath();
			//System.out.println(path);
			file = ImageIO.read(new File("../" + imageFile));
		} catch (IOException e) {
		    System.out.println("No input image found");
		    return;
		} /*catch (URISyntaxException e) {
			// TODO Auto-generated catch block
			e.printStackTrace();
		}*/
		
		Image2d img = new Image2d(file);
		addHexagons(file, img);
		Image2d img_constraints=img.clone(), img_solved= img.clone();		
		
		for(int i=0; i<adjList.size(); i++)		
			printLabel(img, i, "v"+Integer.toString(i+1));	///adding number labels to initial image
		
		new Image2dViewer(img);  /// printing grid with no solution		
		Scanner sc = new Scanner(System.in);
		do {
			System.out.printf("Source (1 to %d): ", adjList.size());
			
			source = sc.nextInt();
			System.out.printf("Target (1 to %d and different of %d): ", adjList.size(), source);
			destination = sc.nextInt(); // user chooses source and destination
			
			source--;
			destination--;
			
			writeGraph();			
			try {
				//String path = SolverInterface.class.getProtectionDomain().getCodeSource().getLocation().toURI().getPath();
				ProcessBuilder pb = new ProcessBuilder("./RikudoSolver", graphFile,  solutionFile);
			    Process p = pb.start();     // Start the process.
			    p.waitFor();                // Wait for the process to finish.
			    
			    
			} catch (Exception e) {
			    e.printStackTrace();
			}
			readSolution();  // read constraints and solution
			if(path.size()==0)
				System.out.println("This yields no solution. Try again.");
		}while(path.size()==0);
	
		printConstraints(img_constraints);
		printPath(img_solved);
		
		new Image2dViewer(img_constraints); // printing grid with constraints
		new Image2dViewer(img_solved); // printing grid with solution
				

	}
	
	private void addHexagons(BufferedImage file, Image2d img){		
		for(int i=0; i*img.getHexSize()<file.getHeight();i++)
		{
			int initialX = -i/2;
			int y = i+1;
			for(int j=0; j*img.getHexSize()<file.getWidth(); j++)
			{			
				int x = initialX+j;
				Offset position = (new Axial(x, y)).toOffset(img.getHexSize());
				double minY = position.y-img.getHexSize(), maxY = position.y+img.getHexSize();
				double minX = position.x-img.getHexSize()*Math.sqrt(3)/2 , maxX = position.x+img.getHexSize()*Math.sqrt(3)/2;
				
				if(0<=minX && maxX<img.getWidth() && 
					0<=minY && maxY<img.getHeight())
				{
					int RGB = file.getRGB(i*img.getHexSize(), j*img.getHexSize());
					int red = (RGB>>16)&255,green= (RGB>>8)&255,blue= (RGB)&255;
					if(red==0 && green==0 && blue==0)
						addHexagon(img, x, y);
				}						
			}
		}		
					
	}
	
	private void addHexagon(Image2d img, int x, int y) {
		Axial axial = new Axial(x, y);
		
		if(axialToInt.containsKey(axial))
			return;
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
		graphText.append("-1\n");
		graphText.append(String.format("%d %d\n", source, destination));
		try {
			Files.write(Paths.get(graphFile), graphText.toString().getBytes());
		}
		catch(Exception e) {
			System.out.println("Error writing graph to file " + graphFile);
		}
	}
	
	
	private void readSolution() {
		path = new LinkedList<>();
		diamonds = new ArrayList<>();
		partialMap = new TreeMap<>();
		int u, v, position;		
		try {
			File file = new File(solutionFile);
			Scanner sc = new Scanner(file);
			
			// reading hamiltonian path
			u = sc.nextInt();
			while(u!=-1) {
				path.add(u);
				u = sc.nextInt();
			}
			//reading partial map
			u = sc.nextInt();
			while(u!=-1) {
				position = sc.nextInt();
				partialMap.put(u, position);
				u = sc.nextInt();
			}
			//reading diamonds
			u = sc.nextInt();
			while(u!=-1) {
				v = sc.nextInt();
				diamonds.add(new Diamond(u, v));
				u = sc.nextInt();
			}
		}
		catch(Exception e) {
			System.out.println("Error reading path from file " + solutionFile);
		}
	}
	
	private void printConstraints(Image2d img) {
		for(Map.Entry<Integer, Integer> entry : partialMap.entrySet())
		{
			int u = entry.getKey(), position = entry.getValue();
			printLabel(img, u, Integer.toString(position));
		}
		
		for(int i=0; i<diamonds.size(); i++)
		{
			printDiamond(img, diamonds.get(i).first, diamonds.get(i).second);
		}
	}
	
	private void printDiamond(Image2d img, int u, int v)
	{
		Offset center1 = intToAxial.get(u).toOffset(img.getHexSize());
		Offset center2 = intToAxial.get(v).toOffset(img.getHexSize());
		Offset center3 = new Offset((center1.x+center2.x)/2, ((center1.y+center2.y)/2));
		img.addHexagon(center3, img.getHexSize()/3, Color.BLACK, Color.BLACK);		
	}
	
	private void printPath(Image2d img) {
		ListIterator<Integer> it = path.listIterator();
		int prev = it.next();
		while(it.hasNext()) {
			int cur = it.next();
			Offset prevOffset = intToAxial.get(prev).toOffset(img.getHexSize());
			Offset curOffset = intToAxial.get(cur).toOffset(img.getHexSize());			
			img.addSegment(prevOffset.x, prevOffset.y, curOffset.x, curOffset.y, 5, Color.BLACK);			
			prev = cur;
		}		
		for(int i=0; i<path.size(); i++)
			printLabel(img, path.get(i), Integer.toString(i+1));
	}	
	        
	private void printLabel(Image2d img, int vertex, String string) {
		Offset offset = intToAxial.get(vertex).toOffset(img.getHexSize());
    	img.addText(new Text(string, offset.x, offset.y));
    }

	
}

