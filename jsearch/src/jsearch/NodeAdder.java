package jsearch;

import java.io.*;
import java.util.*;

import org.jdom2.*;
import org.jdom2.input.SAXBuilder;
import org.jdom2.output.Format;
import org.jdom2.output.XMLOutputter;

public class NodeAdder {
	// Records index of last node in root, so we can insert new nodes nicely after the last
    private Integer lastNodeIndex;
    // Root element
    private Element OSMroot;
    // Table of root nodes for convenient lookup
    private Hashtable<Long, Element> rootNodes;
    // List of new nodes to be inserted in the root
    private ArrayList<Element> newNodes;
    // ID counter that starts after the last ID of the original OSM file.
    // Each z10, z11 and z12 tile is never produced from more than one z9 tile,
    // so there should be no ID collisions.
    private Long nextID;
    // z10s, z11s, z12s hash maps, used for flagging additional tiles for rendering.
    private HashMap<Integer, Boolean> z10s, z11s, z12s;
    
    public NodeAdder (HashMap<Integer, Boolean> z10s, HashMap<Integer, Boolean> z11s, HashMap<Integer, Boolean> z12s) {
    	this.z10s = z10s;
    	this.z11s = z11s;
    	this.z12s = z12s;
    }
    
    public void addNodes(File inputFile, File outputFile) throws IOException, JDOMException, DataConversionException {
	    SAXBuilder saxBuilder = new SAXBuilder();
	    Document document = saxBuilder.build(inputFile);
	    
	    OSMroot = document.getRootElement();
	    
	    rootNodes = new Hashtable<Long, Element>();
	    newNodes = new ArrayList<Element>();
	    
	    nextID = 0L;
	    
	    for (Element rootChild : OSMroot.getChildren()) {
	    	Long elementID = -1L;
	    	Attribute idAttribute = rootChild.getAttribute("id");
	    	if (idAttribute != null) {
	    		elementID = idAttribute.getLongValue();
	    	}
	    	if (nextID <= elementID) {
	    		nextID = elementID + 1;
	    	}
	    	if (rootChild.getName() == "node") {
	    		rootNodes.put(elementID, rootChild);
	    		lastNodeIndex = OSMroot.indexOf(rootChild);
	    	}
	    }

	    for (Element rootChild : OSMroot.getChildren("way")) {
	    	Element lastnd = null;
	    	LinkedHashMap<Integer, ArrayList<Element>> ndsToAdd = new LinkedHashMap<Integer, ArrayList<Element>>();
	    	for (Element wayChild : rootChild.getChildren("nd")) {
	    		if (lastnd != null) {
	    			Integer nd2Index = rootChild.indexOf(wayChild);
	    			Element currentnd = wayChild;
	    			createMissingNodes(ndsToAdd,
	    					resolveNdRef(lastnd.getAttribute("ref").getLongValue()),
	    					resolveNdRef(currentnd.getAttribute("ref").getLongValue()), nd2Index);
	    		}
	    		lastnd = wayChild;
	    	}
	    	
	    	Integer indexOffset = 0;
	    	Integer currentIndex = 0;
	    	for (Integer newNdsIndex : ndsToAdd.keySet()) {
	    		ArrayList<Element> newNds = ndsToAdd.get(newNdsIndex);
	    		currentIndex = newNdsIndex;
	    		for (Element nd : newNds) {
	    			rootChild.addContent(currentIndex + indexOffset, nd);
	    			currentIndex += 1;
	    		}
	    		indexOffset += newNds.size();
	    	}
	    }

	    for (Element node : newNodes) {
	    	lastNodeIndex += 1;
	    	OSMroot.addContent(lastNodeIndex, node);
	    }
	    
	    XMLOutputter serializer = new XMLOutputter();
	    FileOutputStream outputStream = new FileOutputStream(outputFile);
	    try {
	    	serializer.setFormat(Format.getPrettyFormat());
	    	serializer.output(document, outputStream);
	    } catch (IOException e) {
	    	System.err.println(e);
	    }
    }
    
    private void addNodeToRoot(Element nodeElement) throws DataConversionException{
    	rootNodes.put(nodeElement.getAttribute("id").getLongValue(), nodeElement);
    	newNodes.add(nodeElement);
    }
    
    private Element resolveNdRef(Long refnr) {
    	return rootNodes.get(refnr);
    }
    
    private void createMissingNodes(LinkedHashMap<Integer, ArrayList<Element>> addedNds, Element node1, Element node2, Integer nd2Index) throws DataConversionException {
        Double lon1 = node1.getAttribute("lon").getDoubleValue();
        Double lat1 = node1.getAttribute("lat").getDoubleValue();
        Double lon2 = node2.getAttribute("lon").getDoubleValue();
        Double lat2 = node2.getAttribute("lat").getDoubleValue();
        Double xco1 = TileConversion.lon2xtileco(lon1, 12);
        Double yco1 = TileConversion.lat2ytileco(lat1, 12);
        Double xco2 = TileConversion.lon2xtileco(lon2, 12);
        Double yco2 = TileConversion.lat2ytileco(lat2, 12);
        Double distance = Math.sqrt(Math.pow((xco2-xco1), 2.0) + Math.pow((yco2-yco1), 2.0));
        Integer numExtraNodes = 0;
        
        // If distance between two nodes in a way is above 3 tiles,
        // we add 1 node for each tile beyond that 
        if (distance > 3) {
        	numExtraNodes = ((int) Math.floor(distance))-2;
        	Double stepX = (xco2-xco1) / (numExtraNodes + 1);
        	Double stepY = (yco2-yco1) / (numExtraNodes + 1);
        	
        	ArrayList<Element> nodesToAdd = new ArrayList<Element>();
        	for (int i = 0; i < numExtraNodes; i++) {
                Double lon = TileConversion.tileco2lon(xco1 + stepX * (i+1), 12);
                Double lat = TileConversion.tileco2lat(yco1 + stepY * (i+1), 12);
                Element nodeElement = new Element("node");
                nodeElement.setAttribute("id", nextID.toString());
                nodeElement.setAttribute("lat", lat.toString());
                nodeElement.setAttribute("lon", lon.toString());
                nodeElement.setAttribute("version", "1");
                nodeElement.setAttribute("timestamp", "0000-01-27T21:03:16Z");
                nodeElement.setAttribute("changeset", "0");
                nodeElement.setAttribute("uid", "0");
                nodeElement.setAttribute("user", "");
                addNodeToRoot(nodeElement);
                Element ndElement = new Element("nd");
                ndElement.setAttribute("ref", nextID.toString());
                
                // Flag additional tiles for rendering
                Integer xtile = TileConversion.lon2xtile(lon, 12);
                Integer ytile = TileConversion.lat2ytile(lat, 12);
				z10s.put(((xtile / 4) * 1024) + (ytile / 4), true);
				z11s.put(((xtile / 2) * 2048) + (ytile / 2), true);
				for (int x = xtile - 1; x <= xtile + 1; x++) {
					for (int y = ytile - 1; y <= ytile + 1; y++) {
						if ((y >= 0) && (y <= 4095))
							z12s.put((((x < 0) ? 4095 : (x > 4095) ? 0 : x) * 4096) + y, true);
					}
				}
				                
                nodesToAdd.add(ndElement);
                nextID +=1;
        	}
        	addedNds.put(nd2Index, nodesToAdd);
        }
    }
}
