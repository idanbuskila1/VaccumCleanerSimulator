import pygraphviz as pgv

# Create a new directed graph
graph = pgv.AGraph(directed=True)

# Define nodes
graph.add_node("Main", shape="ellipse")
graph.add_node("House", shape="box")
graph.add_node("VacuumCleaner", shape="box")
graph.add_node("CleaningAlgorithm", shape="box")

# Add edges (primary interactions)
graph.add_edge("Main", "House", label="init House")
graph.add_edge("Main", "VacuumCleaner", label="init VacuumCleaner")
graph.add_edge("Main", "CleaningAlgorithm", label="init CleaningAlgorithm")
graph.add_edge("Main", "VacuumCleaner", label="cleanHouse")
graph.add_edge("VacuumCleaner", "House", label="getDockingStationLoc")
graph.add_edge("VacuumCleaner", "CleaningAlgorithm", label="getNextMove")
graph.add_edge("CleaningAlgorithm", "VacuumCleaner", label="chooseNextDirection")
graph.add_edge("Main", "main", label="makeOutputFile")

# Draw the graph
graph.layout(prog="dot")
graph.draw("sequence_diagram.png")