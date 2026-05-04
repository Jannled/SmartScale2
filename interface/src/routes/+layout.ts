import type { LayoutLoad } from './$types';

// This can be false if you're using a fallback (i.e. SPA mode)
export const prerender = false;
export const ssr = false;

export const load = (async ({ fetch }) => {
	const result = await fetch('/rest/features');
	const item = await result.json();
	return {
		features: item,
		title: 'SmartScale 2',
		github: 'Jannled/SmartScale2',
		copyright: '',
		appName: 'SmartScale 2'
	};
}) satisfies LayoutLoad;
