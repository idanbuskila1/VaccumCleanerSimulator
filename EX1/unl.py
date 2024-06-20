import pygraphviz as pgv

# Create a new directed graph
graph = pgv.AGraph(directed=True)

# Define nodes
graph.add_node("Main", shape="ellipse")
graph.add_node("House", shape="box")
graph.add_node("VaccumCleaner", shape="box")
graph.add_node("CleaningAlgorithm", shape="box")

# Add edges (interactions)
graph.add_edge("Main", "House", label="initialize House with docking location and map")
graph.add_edge("Main", "VaccumCleaner", label="initialize VaccumCleaner with max battery steps and docking location")
graph.add_edge("Main", "CleaningAlgorithm", label="initialize CleaningAlgorithm with max battery steps and max steps")

graph.add_edge("Main", "VaccumCleaner", label="cleanHouse(House, CleaningAlgorithm, maxSteps)")

graph.add_edge("VaccumCleaner", "House", label="getDockingStationLoc()")
graph.add_edge("VaccumCleaner", "House", label="getTotalDirtLeft()")
graph.add_edge("VaccumCleaner", "House", label="getDirtLevel(curLoc)")
graph.add_edge("VaccumCleaner", "House", label="isWallInDirection(NORTH, curLoc)")
graph.add_edge("VaccumCleaner", "House", label="isWallInDirection(SOUTH, curLoc)")
graph.add_edge("VaccumCleaner", "House", label="isWallInDirection(WEST, curLoc)")
graph.add_edge("VaccumCleaner", "House", label="isWallInDirection(EAST, curLoc)")

graph.add_edge("VaccumCleaner", "CleaningAlgorithm", label="getNextMove(dirtSensor, batterySensor, wallSensor)")

graph.add_edge("CleaningAlgorithm", "VaccumCleaner", label="chooseNextDirection(wallSensor)")

graph.add_edge("VaccumCleaner", "VaccumCleaner", label="stay(House)")
graph.add_edge("VaccumCleaner", "VaccumCleaner", label="move(dir)")

graph.add_edge("Main", "VaccumCleaner", label="makeOutputFile(VaccumCleaner, House, stepLog)")

# Draw the graph
graph.layout(prog="dot")
graph.draw("sequence_diagram.png")