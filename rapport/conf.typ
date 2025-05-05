// set language to french
#set text(
	lang: "fr",
)

#let conf(title: none, project_name: none, authors: (), abstract: [], doc) = {
  	// Set and show rules from before.
	set align(center)
	text(17pt)[
    		*#title #project_name*
	]

	set text(
		lang: "fr",
	)


	let count = authors.len()
	let ncols = calc.min(count, 3)
	grid(
		columns: (1fr,) * ncols,
		row-gutter: 24pt,
		..authors.map(author => [
			#author.name \
			#author.affiliation \
		]),
	)

	set align(left)
	par(justify: false)[
		*Résumé* \
		#abstract
	]

	set align(left)
	doc
}