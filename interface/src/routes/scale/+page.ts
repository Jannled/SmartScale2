import type { PageLoad } from './$types';

export const load = (async ({ fetch }) => {
	return {
		title: 'Smart Scale'
	};
}) satisfies PageLoad;
